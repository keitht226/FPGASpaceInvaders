#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "uart_functions.h"
#include "ScreenFunctions.h"

//moves about every half_second
#define ALIEN_SPEED 50
//faster than alien speed by considerable amount
#define MOTHERSHIP_SPEED 10
#define EXPLODE_TIME 200
#define MOTHERSHIP_MIN 5 //lowest number of seconds until next mothership
#define MOTHERSHIP_MAX 20//highest number of sec until next mothership

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.
static uint16_t Timer;
static uint16_t mothershipTimer;

//update bullets, move aliens, move motherhsip, make new alien bullet if less than 4 on screen
void timer_interrupt_handler(){
  //ensure random for mothership and alien bullets
  srand((unsigned)time(&timer));
  //move alien block
  if(!(timer%ALIEN_SPEED))
    moveAlienBlock();
  //perform tank animation
  if(running){//running set in uart_functions.c
    timer = 1;//reset timer
    while(timer < EXPLODE_TIME){//explode for two seconds
      //TODO'splosion animation
    }
    globals_tankDeath = stopped; 
  }
  //if mothership is present, move mothership
  if(globals_mothershipState == ALIVE && !(timer % MEOTHERSHIP_SPEED)){
    mothershipPosition += MOTHERSHIP_MOVEMENT;
    //TODO draw mothership
  }
  //draw mothership if mothershipSpawnCounter reached
  if(!(timer % mothershipSpawnCounter)){
    globals_mothershipState = ALIVE;
    mothershipTimer = 0;//initialize/reset timer
    //TODO draw mothership
  }
  //inc timer and protect against overflow. 
  if(timer == UINT16_MAX){ 
    timer = 1;//Reset timer to 1 so as not to mess up mod operations
  }else{
    ++timer;
  }
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
