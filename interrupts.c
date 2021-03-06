#include "interrupts.h"
#include "xparameters.h"
#include "sounds.h"
#include "xac97_l.h"
#include "pit3.h"

#define DEATH_FOR_FASEST 34
#define DEATH_FOR_MEDIUM 17
#define INITIAL_TANK_POSITION 320
#define MOTHERSHIP_EDGE_CORRECTION 6
#define ALIEN_BULLET_MIN 75
#define ALIEN_BULLET_MAX 200
#define MAX_NUM_SAMPLES 100

static uint16_t timer = 1; //general timer. Constantly running in FIT
static uint16_t mothershipTimer = 1; //timer specifically for mothership. Helps accomodate spontaneous appearance
static bool first = true; //help setup the FIT the first time
//static uint32_t mothSoundi, alienSoundi, explosionSoundi, shootSoundi, mothExplosionSoundi = 0;
static int*samples = 0;
static int num_samples = 0;
static bool readyForSound = true;

/*remove when python script works */\
static int* explosionSamples = explosion_array;
static int  explosionNumSamples = 4080;

static int* alienSamples = fastinvader2_array;
static int  alienNumSamples = 1042;

static int* mothExplosionSamples = explosion_array;
static int  mothExplosionNumSamples = 8731;

static int* mothershipSamples = ufo_highpitch_array;
static int  mothershipNumSamples = 1802;

static int* tankFireArray = tankFireSound;
static int  tankFireSamples = 4080;

static int* invaderKilledArray = invaderkilled_array;
static int  invaderKilledSamples = 3377;

static int* alienFireArray = shoot_array;
static int  alienFireSamples = 4080;


//update bullets, move aliens, move motherhsip, make new alien bullet if less than 4 on screen
void timer_interrupt_handler(){

	//xil_printf("Interrupt Handler\n\r");
	//xil_printf("firstSound: %d\n\r",firstSound);
	if(globals_tankDeath == running){
			if(readyForSound){
				samples = explosionSamples;
				num_samples = explosionNumSamples;
				readyForSound = false;
			}
            XGpio_InterruptGlobalDisable(&gpPB); // Turn off all PB interrupts for now.
            if(first){
                  timer = 1;
                  first = false;
            }
  //perform tank animation-----------------------------------------------------------------------
            if(timer < EXPLODE_TIME){//explode for two seconds
                    if(!(timer % 2))
                            write_tank_explosion1();
                    else
                            write_tank_explosion2();
            }
            else{
            		//explosionSoundIndex = 0;
                    first = true;
                    timer = 1;
                    globals_tankDeath = stopped;
                    write_tank_black();
                    globals_setTankPosition(INITIAL_TANK_POSITION);
                    write_tank_to_memory();
                    XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
            }
	}
        //tank death animation isn't happening. Do other stuff
	else{

	    u32 currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
	    // allow user to use buttons, even multiple at once. All three at once don't move the tank but do shoot a bullet
	    if(!(timer % TANK_SPEED)){
			switch(currentButtonState){
			  case 8:
				moveTankLeft();
				break;
			  case 1:
				newTankBullet();
				if(readyForSound){
					samples = tankFireArray;
					num_samples = tankFireSamples;
					readyForSound = false;
				}
				break;
			  case 2:
				moveTankRight();
				break;
			  case 9:
				moveTankLeft();
				newTankBullet();
				if(readyForSound){
					samples = tankFireArray;
					num_samples = tankFireSamples;
					readyForSound = false;
				}
				break;
			  case 3:
				moveTankRight();
				newTankBullet();
				if(readyForSound){
					samples = tankFireArray;
					num_samples = tankFireSamples;
					readyForSound = false;
				}
				break;
			  case 11:
				newTankBullet();
				if(readyForSound){
					samples = tankFireArray;
					num_samples = tankFireSamples;
					readyForSound = false;
				}
				break;
			  case 4:
				  //volume up
				  break;
			  case 16:
				  //volume down
				  break;
			  default:
				break;
			}
	    }

	  //ensure random for mothership and alien bullets
	  srand(timer);

	//if mothership is present, move mothership-----------------------------------------------------
	  if(globals_mothershipState == ALIVE && !(timer % MOTHERSHIP_SPEED)){
                //assign new position for mothership
		mothershipPosition += MOTHERSHIP_MOVEMENT;
                //mothership at the edge? 
		if(mothershipPosition + MOTHERSHIP_WIDTH >= X_MAX-MOTHERSHIP_EDGE_CORRECTION){
			mothershipSpawnCounter = rand() % (MOTHERSHIP_MAX + 1 - MOTHERSHIP_MIN) + MOTHERSHIP_MIN;//create a new spawn timer
			globals_mothershipState = DEAD; //erase mothership
			write_mothership_black_to_memory();
			mothershipPosition = 0; //reset mothership position
		}
		else
			write_mothership_to_memory(); //if not at edge, move the ship on screen
			if(readyForSound){
				samples = mothershipSamples;
				num_samples = mothershipNumSamples;
				readyForSound = false;
			}
	  }


	//draw mothership if mothershipSpawnCounter reached---------------------------------------------
	  if(!(mothershipTimer % mothershipSpawnCounter)){
		globals_mothershipState = ALIVE; //set ship to alive
		mothershipTimer = 1;//initialize/reset timer
		write_mothership_to_memory(); //draw on screen
	  }

	//stall deleting alien long enough to see explosion---------------------------------------------
	  if(beginAlienExplosion){
                  //stall till explosion done
		  if(alienExplodeCounter < ALIEN_EXPLODE_TIME){
			  ++alienExplodeCounter;
		  }
		  if(alienExplodeCounter == ALIEN_EXPLODE_TIME){
			  write_alien_dead_to_memory(globals_alien); //erase explosion
			  alienExplodeCounter = 1; //reset timer
			  beginAlienExplosion = false; //end delay
		  }
	  }

//killed mother ship stuff-------------------------------------------------------------------------
	  if(beginMotherExplosion){

		  if(readyForSound){
			  samples = mothExplosionSamples;
			  num_samples = mothExplosionNumSamples;
			  readyForSound = false;
		  }
		  //draw 150 in place of mothership
		  write_mothership_hit_score_to_memory();
                  //delay long enough for user to see
		  if(alienExplodeCounter < MOTHERSHIP_EXPLODE_TIME){
			  ++alienExplodeCounter;
		  }
                  //delay limit reached
		  if(alienExplodeCounter == MOTHERSHIP_EXPLODE_TIME){
			  write_mothership_black_to_memory(); //erase score
			  alienExplodeCounter = 1; //reset counter
			  beginMotherExplosion = false; //no longer exploding
			  mothershipSpawnCounter = rand() % (MOTHERSHIP_MAX + 1 - MOTHERSHIP_MIN) + MOTHERSHIP_MIN;//make new spawn counter
			  mothershipPosition = 0;//reset position
		  }
	  }

	//move alien block----------------------------------------------------------------------------
		//if large amount of aliens are dead, move at fastest rate
	  if(dead_alien_count > DEATH_FOR_FASEST){
		  if(!(timer%ALIEN_SPEED3)){
			  moveAlienBlock();

			  if(readyForSound){
				  samples = alienSamples;
				  num_samples = alienNumSamples;
				  readyForSound = false;
			  }

		  }
	  }
		  //if medium amount dead, move at medium speed
	  else if(dead_alien_count > DEATH_FOR_MEDIUM){
		  if(!(timer%ALIEN_SPEED2)){
			  moveAlienBlock();

			  if(readyForSound){
				  samples = alienSamples;
				  num_samples = alienNumSamples;
				  readyForSound = false;
			  }

		  }
		  //otherwise move at slowest rate
	  }else{
		  if(!(timer%ALIEN_SPEED1)){
			  moveAlienBlock();

			  if(readyForSound){
				  samples = alienSamples;
				  num_samples = alienNumSamples;
				  readyForSound = false;
			  }

		  }
	  }

	//update bullets--------------------------------------------------------------------------------
	  if(!(timer % BULLET_SPEED)){
		  updateBullets();
	  }


	//create new alien bullet-----------------------------------------------------------------------
	  if(!(timer % (rand()%(ALIEN_BULLET_MAX+1-ALIEN_BULLET_MIN)+ALIEN_BULLET_MIN))){//new alien bullet from 2-5 seconds
		 newAlienBullet();
	  }

	//inc mothership timer. Cannot overflow under correct operation---------------------------------
	  if(globals_mothershipState == DEAD)
		  ++mothershipTimer;//mothership needs its own timer. Appears at unpredictable times

	}

//inc timers-------------------------------------------------------------------------------------
  if(timer == UINT16_MAX){ 
    timer = 1;//Reset timer to 1 so as not to mess up mod operations
  }else{
    ++timer;
  }

}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
// move tank. Creat new tank bullet
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.

  XGpio_InterruptClear(&gpPB, 0xFFFFFFFF);            // Ack the PB interrupt.
  XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
}

//helper function for audio
static void playSound(int* samples, int num_samples){
	uint32_t stop = 0;
	static uint32_t i = 0;
	if(!readyForSound){
		while(i < num_samples && stop != MAX_NUM_SAMPLES){
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, *(samples+i) << 16 | *(samples+i));
			i++;
			stop++;
		}
	}else{
		while(stop != MAX_NUM_SAMPLES){
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, 0);
			stop++;
		}
		return;
	}
	if(i == num_samples){
		i = 0;
		readyForSound = true;
		num_samples = 0;
	}
	return;
}

void sound_interrupt_handler(){
	//load sound into fifo------------------------------------------------------------------------------
	playSound(samples, num_samples);
	return;
}

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr) {
	int intc_status = XIntc_GetIntrStatus(XPAR_INTC_0_BASEADDR);
	// Check the FIT interrupt first.
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	//xil_printf("intc_status: %d\n\r", intc_status);
	if (intc_status & XPAR_PIT3_0_PIT_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT3_0_PIT_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
		pb_interrupt_handler();
	}
	if (intc_status & XPAR_AXI_AC97_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_AC97_0_INTERRUPT_MASK);
		sound_interrupt_handler();
	}
}
