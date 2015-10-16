#include "xgpio.h"          // Provides access to PB GPIO driver.
#include <stdio.h>          // xil_printf and so forth.
#include "platform.h"       // Enables caching and other system stuff.
#include "mb_interface.h"   // provides the microblaze interrupt enables, etc.
#include "xintc_l.h"        // Provides handy macros for the interrupt controller.
#include "uart_functions.h"
#include "ScreenFunctions.h"
#include <stdint.h>
#include "time.h"
#include "globals.h"

//moves about every half_second
#define ALIEN_SPEED 50
//faster than alien speed by considerable amount
#define MOTHERSHIP_SPEED 5
#define EXPLODE_TIME 200 //explode time for tank
#define MOTHERSHIP_MIN 50 //lowest number of seconds until next mothership
#define MOTHERSHIP_MAX 2000//highest number of sec until next mothership
#define ALIEN_EXPLODE_TIME 25 //quarter second
#define BULLET_SPEED 2
#define TANK_SPEED 2

XGpio gpLED;  // This is a handle for the LED GPIO block.
XGpio gpPB;   // This is a handle for the push-button GPIO block.


//update bullets, move aliens, move motherhsip, make new alien bullet if less than 4 on screen
void timer_interrupt_handler();

// This is invoked each time there is a change in the button state (result of a push or a bounce).
// move tank. Creat new tank bullet
void pb_interrupt_handler();

// Main interrupt handler, queries the interrupt controller to see what peripheral
// fired the interrupt and then dispatches the corresponding interrupt handler.
// This routine acks the interrupt at the controller level but the peripheral
// interrupt must be ack'd by the dispatched interrupt handler.
void interrupt_handler_dispatcher(void* ptr);
