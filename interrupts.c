#include "interrupts.h"

static uint16_t timer = 1;
static uint16_t mothershipTimer = 1;

//update bullets, move aliens, move motherhsip, make new alien bullet if less than 4 on screen
void timer_interrupt_handler(){
  //ensure random for mothership and alien bullets
  srand(timer);
  //move alien block
  if(!(timer%ALIEN_SPEED))
    moveAlienBlock();
  //perform tank animation
  if(globals_tankDeath == running){//running set in uart_functions.c
    timer = 1;//reset timer
    if(timer < EXPLODE_TIME){//explode for two seconds
    	if(!(timer % 2))
    		write_tank_explosion1();
    	else
    		write_tank_explosion2();
    }
    globals_tankDeath = stopped; 
  }

  //if mothership is present, move mothership
  if(globals_mothershipState == ALIVE && !(timer % MOTHERSHIP_SPEED)){
    mothershipPosition += MOTHERSHIP_MOVEMENT;
    if(mothershipPosition > X_MAX){
    	globals_mothershipState = DEAD;
    	write_mothership_black_to_memory();
    }
    write_mothership_to_memory();
  }

  //draw mothership if mothershipSpawnCounter reached
  if(!(timer % mothershipSpawnCounter)){
    globals_mothershipState = ALIVE;
    mothershipTimer = 1;//initialize/reset timer
    write_mothership_to_memory();
  }

  //stall deleting alien long enough to see explosion
  if(beginAlienExplosion){
	  if(alienExplodeCounter <= ALIEN_EXPLODE_TIME){
		  ++alienExplodeCounter;
	  }
	  if(alienExplodeCounter == ALIEN_EXPLODE_TIME){
		  beginAlienExplosion = false;
		  alienExplodeCounter = 1;
	  }
  }

  //inc timer and protect against overflow. 
  if(timer == UINT16_MAX){ 
    timer = 1;//Reset timer to 1 so as not to mess up mod operations
  }else{
    ++timer;
  }
  //inc mothership timer. Cannot overflow under correct operation
  if(globals_mothershipState == DEAD)
	  ++mothershipTimer;//mothership needs its own timer. Appears at unpredictable times
}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
// move tank. Creat new tank bullet
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
  u32 currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
  // You need to do something here.
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
  //Down 4 / Up 16 / Hour 8 / Min 1 / Sec 2
  //bit 0 min / 1 sec / 2 down / 3 hour / 4 up
  //if(currentButtonState == 16){
	  	//xil_printf(" %d", currentButtonState);
	   // xil_printf("#");
  //}


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
