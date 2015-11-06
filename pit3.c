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

	//xil_printf("set_delay int right before set: %d\n\r",delay);
	Xil_Out32(XPAR_PIT3_0_BASEADDR+4, delay);
	//xil_printf("set_delay memory right after set: %d\n\r",Xil_In32(XPAR_PIT3_0_BASEADDR+4));
	return;
}

void set_pit_control(int mask){
		PIT3_mWriteSlaveReg0(XPAR_PIT3_0_BASEADDR, 0, mask);
}


void delay_prompt(){
	//setvbuf(stdin, NULL, _IOLBF, 10);
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
	//xil_printf("delay int = %d\n\r",delay);
	//xil_printf("delay reg = %d\n\r"),Xil_In32(XPAR_PIT3_0_BASEADDR+4);
	//setvbuf(stdin, NULL, _IONBF, 0);
}

/************************** Function Definitions ***************************/

