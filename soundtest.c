#include "xparameters.h"
#include "sounds.h"
#include "xac97_l.h"

int main(){
	int i = 0;
	XAC97_HardReset(XPAR_AXI_AC97_0_BASEADDR);

	XAC97_ClearFifos(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVol, AC97_VOL_MIN);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_ExtendedAudioStat, AC97_EXTENDED_AUDIO_CONTROL_VRA);

	XAC97_AwaitCodecReady(XPAR_AXI_AC97_0_BASEADDR);
	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCM_DAC_Rate, AC97_PCM_RATE_11025_HZ);

	XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_AuxOutVol, AC97_VOL_MID);
	  //XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_MasterVolMono, AC97_VOL_MAX);
	  //XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCBeepVol, AC97_VOL_MAX);
	  //XAC97_WriteReg(XPAR_AXI_AC97_0_BASEADDR, AC97_PCMOutVol, AC97_VOL_MAX);
	//XAC97_Delay(100000);

	while(1){
		while(!XAC97_isInFIFOFull(XPAR_AXI_AC97_0_BASEADDR)){
			XAC97_mSetInFifoData(XPAR_AXI_AC97_0_BASEADDR, hey_array[i] << 16 | hey_array[i]);
			if(i < hey_num_samples-1)
				i++;
			else
				i = 0;
			//XAC97_Delay(10);
		}
	}
	return 0;
}
