/*
 * PCM1803A.c
 *
 *  Created on: 30 mag 2019
 *      Author: Alessio
 */

#include <PCM1808.h>
#include "time.h"

void I2S0_Rx_IRQHandler(void)
{
	NVIC->ICER[0] |= (1 << 29); // Disable Interrupt for I2S0

 	I2S0->RCSR |= (1 << 20); // clear flag
}

void PCM1808_init(void)
{
	PORTA->PCR[14] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S RX BCLK (receiver bit clock)
	PORTA->PCR[15] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TX RXD (data in)
	PORTA->PCR[16] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TX RX FS (receiver frame sync)
	PORTA->PCR[17] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for Master Clock

	SIM->SCGC6 |= (1 << 15); // Clock enable for I2S RX module

//	NVIC->IP[29] |= (1 << 11); // Preemptive Priority (2)! (max priority is 0) // NOT USED

//	NVIC->ISER[0] |= (1 << 29); // Enable Interrupt for I2S0

	I2S0->MDR |= (1 << 3) | (1 << 2) | (1 << 0); // Master Clock divide set as 13 (to create a period of 80 ns, 11.5 MHz, for the square wave, so device sample is 44.1 KHz -> 256fs)

	I2S0->RCR2 |= (1 << 25); // bit clock sample input on rising edge

	I2S0->RCR3 |= (1 << 16); // Enable data channel 0 for RX operation

	I2S0->RCR4 |= (1 << 4) | (1 << 3) | (1 << 1); // | (1 << 0); // set MSB first, Frame sync assert one bit before first bit of frame, active low, Frame Sync generated internally (master mode)
	I2S0->RCR4 |= (1 << 28) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8); // FIFO continue on error, Synch Width set as 32 (Synch Width = SYWD + 1)
	I2S0->RCR4 |= (1 << 16); // Set frame with two data words (bit field is number of words - 1)

	I2S0->RCR5 |= (1 << 20) | (1 << 19) | (1 << 18) | (1 << 17) | (1 << 16); // Word 0 length is 32 bit (W0W bit field set as 23)
	I2S0->RCR5 |= (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25) | (1 << 24); // Word N length is 32 bit (WNW bit field set as 23)
	I2S0->RCR5 |= (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9) | (1 << 8); // First bit index is 32

	I2S0->MCR |= (1 << 30); // MCLK Output Enable

	I2S0->RCSR |= (1 << 31) | (1 << 30) | (1 << 29) | (1 << 28) | (1 << 12); // Enable RX, RX enable in stop mode, RX enable in debug, Bit Clock Enable, enable word start interrupt
}

void PCM1808_receiveWaveForm(int32_t* dataAudioL, int32_t* dataAudioR, uint16_t numberOfSampleInASecond)
{
	int32_t buffer = 0;

//	for(int i = 0; i < numberOfSampleInASecond; i++)
//	{
//		dataAudioL[i] = 0;
//		dataAudioR[i] = 0;
//	}

	for(int i = 0; i < numberOfSampleInASecond; i++)
	{
		while (!(I2S0->RCSR & (1 << 17))) // wait until FIFO is full
		{

		}

		buffer = I2S0->RDR[0];

		if((buffer & (1 << 31)) == (1 << 31))
		{
			dataAudioL[i] =  (buffer >> 8) | (1 << 31); //
		}
		else
		{
			dataAudioL[i] = buffer >> 8;
		}

		buffer = I2S0->RDR[0];

		if((buffer & (1 << 31)) == (1 << 31))
		{
			dataAudioR[i] =  (buffer >> 8) | (1 << 31); //
		}
		else
		{
			dataAudioR[i] = buffer >> 8;
		}
	}

}
