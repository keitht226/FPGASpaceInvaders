#include "interrupts.h"

static uint16_t timer = 1;
static uint16_t mothershipTimer = 1;
static bool first = true;

//update bullets, move aliens, move motherhsip, make new alien bullet if less than 4 on screen
void timer_interrupt_handler(){


	if(globals_tankDeath == running){
		  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.

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
			first = true;
			timer = 1;
			globals_tankDeath = stopped;
			write_tank_black();
			globals_setTankPosition(320);
			write_tank_to_memory();
			XGpio_InterruptGlobalEnable(&gpPB);                 // Re-enable PB interrupts.
		}
	}
	else{

	    u32 currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
	    // You need to do something here.
	    if(!(timer % TANK_SPEED)){
			switch(currentButtonState){
			  case 8:
				moveTankLeft();
				break;
			  case 1:
				newTankBullet();
				break;
			  case 2:
				moveTankRight();
				break;
			  default:
				break;
			}
	    }

	  //ensure random for mothership and alien bullets
	  srand(timer);

	//move alien block----------------------------------------------------------------------------
	  if(dead_alien_count > 34){
		  if(!(timer%ALIEN_SPEED3)){
			  moveAlienBlock();
		  }
	  }
	  else if(dead_alien_count > 17){
		  if(!(timer%ALIEN_SPEED2)){
			  moveAlienBlock();
		  }
	  }else{
		  if(!(timer%ALIEN_SPEED1)){
			  moveAlienBlock();
		  }
	  }


	//if mothership is present, move mothership-----------------------------------------------------
	  if(globals_mothershipState == ALIVE && !(timer % MOTHERSHIP_SPEED)){
		mothershipPosition += MOTHERSHIP_MOVEMENT;
		if(mothershipPosition > X_MAX){
			globals_mothershipState = DEAD;
			write_mothership_black_to_memory();
		}
		write_mothership_to_memory();
	  }


	//draw mothership if mothershipSpawnCounter reached---------------------------------------------
	  if(!(timer % mothershipSpawnCounter)){
		globals_mothershipState = ALIVE;
		mothershipTimer = 1;//initialize/reset timer
		write_mothership_to_memory();
	  }

	//stall deleting alien long enough to see explosion---------------------------------------------
	  if(beginAlienExplosion){
		  if(alienExplodeCounter < ALIEN_EXPLODE_TIME){
			  ++alienExplodeCounter;
		  }
		  if(alienExplodeCounter == ALIEN_EXPLODE_TIME){
			  write_alien_dead_to_memory(globals_alien);
			  alienExplodeCounter = 1;
			  beginAlienExplosion = false;
		  }
	  }


	//update bullets--------------------------------------------------------------------------------
	  if(!(timer % BULLET_SPEED)){
		  updateBullets();
	  }


	//create new alien bullet-----------------------------------------------------------------------
	  if(!(timer % (rand()%(200+1-75)+75))){//new alien bullet from 2-5 seconds
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
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
		pb_interrupt_handler();
	}
}
