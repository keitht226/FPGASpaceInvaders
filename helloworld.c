#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "pit3.h"

#define SECOND 100
#define HALF_SECOND SECOND/2
#define DEBOUNCE 5

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.
u32 timer;
u32 btn_values_old;
u32 btn_values_new;
u32 debounce_timer;
u32 hold_timer;
u32 second, minute, hour;
// This is invoked in response to a timer interrupt.
// It does 2 things: 1) debounce switches, and 2) advances the time.

void timer_interrupt_handler() {
	btn_values_new = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
	//Button was pushed
	if(btn_values_new != 0 ){
		//If same combination of buttons was being held
		if(btn_values_new == btn_values_old){
			//If held for a second
			if(hold_timer == SECOND){
			//UP/DOWN implementation
				//Check every half-second for auto-inc
				if(timer == HALF_SECOND || timer == SECOND-1) {
					//Up being pushed
					if(btn_values_new & 16) {
						if(btn_values_new & 8) { //Hour
							if(hour != 11){
								++hour;
							}
							else{
								hour = 1;
							}
						}
						if(btn_values_new & 1) { //Minute
							if(minute != 59){
								++minute;
							}
							else{
								minute = 1;
							}

						}
						if(btn_values_new & 2) { //Sec
							if(second != 59){
								++second;
							}
							else{
								second = 1;
							}
						}

						xil_printf("\r%02d:%02d:%02d ",hour,minute,second);

				}// end of up button being pushed if


				//Check if Down is being pushed
				if(btn_values_new & 4) {
						if(btn_values_new & 8) { //Hour
							if(hour != 1){
								--hour;
							}
							else{
								hour = 12;
							}
						}
						if(btn_values_new & 1) { //Minute
							if(minute != 0){
								--minute;
							}
							else{
								minute = 59;
							}

						}
						if(btn_values_new & 2) { //Sec
							if(second != 0){
								--second;
							}
							else{
								second = 59;
							}
						}

				xil_printf("\r%02d:%02d:%02d ",hour,minute,second);

				}// end of up button being pushed if



				}
			}
			else{
				hold_timer++;
			}

		}
			//add holding 0x

		else{//pushed but haven't held it for a second
			hold_timer = 0;
			if(debounce_timer < DEBOUNCE){
				debounce_timer++;
				return;
			}
			else{
				if(btn_values_new != 0){
					if(btn_values_new & 16){//UP
						if(btn_values_new & 8) { //Hour
							if(hour != 11){
								++hour;
							}
							else{
								hour = 1;
							}
						}
						if(btn_values_new & 1) { //Minute
							if(minute != 59){
								++minute;
							}
							else{
								minute = 1;
							}

						}
						if(btn_values_new & 2) { //Sec
							if(second != 59){
								++second;
							}
							else{
								second = 1;
							}
						}
					}
					else if(btn_values_new & 4){//Down
						if(btn_values_new & 8) { //Hour
							if(hour != 1){
								--hour;
							}
							else{
								hour = 12;
							}
						}
						if(btn_values_new & 1) { //Minute
							if(minute != 0){
								--minute;
							}
							else{
								minute = 59;
							}

						}
						if(btn_values_new & 2) { //Sec
							if(second != 0){
								--second;
							}
							else{
								second = 59;
							}
						}
					}
					xil_printf("\r%02d:%02d:%02d ",hour,minute,second);
				}
				debounce_timer = 0;
				btn_values_old = btn_values_new;
			}
		}
	}//End of button was pushed if

	//update time, second by second

	if(timer == 100 && btn_values_new == 0){
		//xil_printf("Time update statement %d\r",btn_values_new);
		timer = 0;
		if(second != 59){
			second++;
		}
		else{
			second = 0;
			if(minute != 59){
				minute++;
			}
			else{
				minute = 0;
				if(hour != 11){
					hour++;
				}
				else{
					hour = 1;
				}
			}
		}
		xil_printf("\r%02d:%02d:%02d ",hour,minute,second);
	}
	if(timer < SECOND){
		timer++;
	}
	else
		timer = 0;
	//xil_printf("%02d:%02d:%02d\r",hour,minute,second);

}

// This is invoked each time there is a change in the button state (result of a push or a bounce).
void pb_interrupt_handler() {
  // Clear the GPIO interrupt.
  XGpio_InterruptGlobalDisable(&gpPB);                // Turn off all PB interrupts for now.
  u32 currentButtonState = XGpio_DiscreteRead(&gpPB, 1);  // Get the current state of the buttons.
  // You need to do something here.

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
	if (intc_status & XPAR_PIT3_0_PIT_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PIT3_0_PIT_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	/*
	if (intc_status & XPAR_FIT_TIMER_0_INTERRUPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_FIT_TIMER_0_INTERRUPT_MASK);
		timer_interrupt_handler();
	}
	*/
	// Check the push buttons.
	if (intc_status & XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK){
		XIntc_AckIntr(XPAR_INTC_0_BASEADDR, XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK);
		pb_interrupt_handler();
	}
}

int main (void) {

	//xil_printf("ctr: %d\n\r",PIT3_mReadSlaveReg0(XPAR_PIT3_0_BASEADDR, 0));
//delay - write
 	delay_prompt();
 	set_pit_control(7);
	//initialize globals
	timer = 0;
	btn_values_old = 0;
	btn_values_new = 0;
	debounce_timer = 0;
	hold_timer = 0;
	minute = 0;
	second = 0;
	hour = 12;

    init_platform();
    // Initialize the GPIO peripherals.
    int success;
    //xil_printf("Check\n\r");
    success = XGpio_Initialize(&gpPB, XPAR_PUSH_BUTTONS_5BITS_DEVICE_ID);
    // Set the push button peripheral to be inputs.
    XGpio_SetDataDirection(&gpPB, 1, 0x0000001F);
    // Enable the global GPIO interrupt for push buttons.
    XGpio_InterruptGlobalEnable(&gpPB);
    // Enable all interrupts in the push button peripheral.
    XGpio_InterruptEnable(&gpPB, 0xFFFFFFFF);

    microblaze_register_handler(interrupt_handler_dispatcher, NULL);
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR,
    		(XPAR_PIT3_0_PIT_INTERRUPT_MASK | XPAR_PUSH_BUTTONS_5BITS_IP2INTC_IRPT_MASK));
    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);
    microblaze_enable_interrupts();
    //setvbuf(stdin, NULL, _IONBF, 0);
    //setvbuf(stdin, NULL, _IOLBF, 10);
	char delay;
    while(1){
    	setvbuf(stdin, NULL, _IOLBF, 1);
    	if((delay = getchar()) == 'q'){
    		set_pit_control(5); //disables interrupts
    		delay_prompt();
    		set_pit_control(7); //enable interrupts
    	}
    	setvbuf(stdin, NULL, _IONBF, 0);
    	// Program never ends.
    }

    cleanup_platform();

    return 0;
}
