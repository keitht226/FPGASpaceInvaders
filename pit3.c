/*****************************************************************************
* Filename:          C:\Users\superman\Desktop\MikeKeith\Lab6\SpaceInvaderTestHW2/drivers/pit3_v1_00_a/src/pit3.c
* Version:           1.00.a
* Description:       pit3 Driver Source File
* Date:              Wed Nov 04 16:01:06 2015 (by Create and Import Peripheral Wizard)
*****************************************************************************/


/***************************** Include Files *******************************/

#include "pit3.h"
#include "xparameters.h"
#include <stdio.h>
#include <stdlib.h>



void set_delay(int delay){
	Xil_Out32(XPAR_PIT3_0_BASEADDR+4, delay);
	return;
}

/*7 sets everything, 5 will disable the interrupt, 2 will enable the interrupt*/
void set_pit_control(int mask){
		PIT3_mWriteSlaveReg0(XPAR_PIT3_0_BASEADDR, 0, mask);
}


void delay_prompt(){
	char delay_array[10];
	int delay = 0;
	xil_printf("Game speed?:\n\r");
	xil_printf("Type a number and then press enter\n\r");
	//wait for input
	int i = 0;
	while((delay_array[i]=getchar()) != '\r'){
		i++;
	}
	delay = atoi(delay_array);
	xil_printf("entered %d\n\r",delay);

	set_delay(delay);
}

/************************** Function Definitions ***************************/

