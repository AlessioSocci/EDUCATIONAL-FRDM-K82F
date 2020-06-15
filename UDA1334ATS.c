/*
 * UDA1334.c
 *
 *  Created on: 13 mag 2019
 *      Author: Alessio
 */

#include <UDA1334ATS.h>
#include "time.h"

SampleFilter f;

float32_t pi = 3.141592653589;

void UDA1334_init(void)
{
	SIM->SCGC6 |= (1 << 15); // Enable clock to I2S module

	PORTA->PCR[5] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TX BCLK (transmitter bit clock)
	PORTA->PCR[12] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TXD (data out)
	PORTA->PCR[13] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TX FS (transmitter frame sync, 10pF capacitor to ground)

//	PORTA->PCR[17] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for Master Clock (not used, it's generated by IC)

	I2S0->TMR = 0; // enable all words in a frame

	I2S0->TCR2 &= ~((1 << 31) | (1 << 30)); // set asynchronous mode (TX is independent by RX, another SAI transmitter or receiver)
	I2S0->TCR2 |= (1 << 24) | (1 << 4); // Bit Clock internally generated so clock is an output, bus clock divided by? ---> for 44100 KHz Sample Rate of the signal input, 24 bit 44.1 kHz word, 2 channel, so Bit Clock must be 2116800 MHz: 75000000 / 2116800 = 35 , so division value = (DIV + 1) * 2 )

	I2S0->TCR3 |= (1 << 16); // Enable TX channel 1

	I2S0->TCR4 |= (1 << 0); // Frame Sync internally generated (Master Mode)
	I2S0->TCR4 |= (1 << 12) | (1 << 11) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 0); // 25 bit clocks frame sync length (Synch Width = SYWD + 1), MSB first, FSync internally generated
	I2S0->TCR4 |= (1 << 16); // Set frame with two data words (bit field is number of words - 1)
	I2S0->TCR4 |= (1 << 28); // FIFO continue on error

	I2S0->TCR5 |=  (1 << 28) | (1 << 27); // Length of word set as 24 bit (bit field is number of bits - 1) (slave device require one dummy bit before every significant frame... so 25 bit is frame length)
	I2S0->TCR5 |= (1 << 20) | (1 << 19); // Length of word 0 set as 24 bit (bit field is number of bits - 1)
	I2S0->TCR5 |= (1 << 12) | (1 << 11); // First bit start at 24 index (MSB first transmitted)

	I2S0->TCSR |= (1 << 31) | (1 << 29) | (1 << 28); // Enable TX, TX enable in stop mode, TX is enable also in debug, enable bit clock
}

void UDA1334_sendData(int32_t* dataAudio_L, int32_t* dataAudio_R, uint16_t numberOfSampleInASecond, uint8_t dsp_on_off, filter_cutfreq fcut)
{
	if (dsp_on_off)
	{
		int32_t dataProcessed_L[numberOfSampleInASecond];
		int32_t dataProcessed_R[numberOfSampleInASecond];

		switch(fcut)
		{
			case f400Hz:

				for(uint16_t s = 0; s < numberOfSampleInASecond; s++)
				{
					dataProcessed_L[s] = 0;
					dataProcessed_R[s] = 0;

					dsp_filter_load(&f, dataAudio_L[s], TAP_NUMBER_1_LPF);

					dataProcessed_L[s] = dsp_filter_process(&f, f400Hz);

					dsp_filter_load(&f, dataAudio_R[s], TAP_NUMBER_1_LPF);

					dataProcessed_R[s] = dsp_filter_process(&f, f400Hz);
				}

				break;

			case f800Hz:

				for(uint16_t s = 0; s < numberOfSampleInASecond; s++)
				{
					dataProcessed_L[s] = 0;
					dataProcessed_R[s] = 0;

					dsp_filter_load(&f, dataAudio_L[s], TAP_NUMBER_2_LPF);

					dataProcessed_L[s] = dsp_filter_process(&f, f800Hz);

					dsp_filter_load(&f, dataAudio_R[s], TAP_NUMBER_2_LPF);

					dataProcessed_R[s] = dsp_filter_process(&f, f800Hz);
				}

				break;

			case f2kHz:

				for(uint16_t s = 0; s < numberOfSampleInASecond; s++)
				{
					dataProcessed_L[s] = 0;
					dataProcessed_R[s] = 0;

					dsp_filter_load(&f, dataAudio_L[s], TAP_NUMBER_3_LPF);

					dataProcessed_L[s] = dsp_filter_process(&f, f2kHz);

					dsp_filter_load(&f, dataAudio_R[s], TAP_NUMBER_3_LPF);

					dataProcessed_R[s] = dsp_filter_process(&f, f2kHz);
				}

				break;
		}

		for(uint16_t s = 0; s < numberOfSampleInASecond; s++)
		{
			for(int i = 0; i < 330; i++); // delay generated by empty cyclic operation, otherwise transmission is degraded by interrupt on timer .

			I2S0->TDR[0] = dataProcessed_L[s] << 8;

			I2S0->TDR[0] = dataProcessed_R[s] << 8;
		}
	}
	else
	{
		for(uint16_t s = 0; s < numberOfSampleInASecond; s++)
		{
			for(int i = 0; i < 330; i++); // delay to follow DAC operation time, generated by empty cyclic operation, otherwise transmission is degraded by interrupt on timer .

			I2S0->TDR[0] = dataAudio_L[s] << 8;
			I2S0->TDR[0] = dataAudio_R[s] << 8;
		}
	}
}

void UDA1334_sendWaveForm(musicalNote note, uint16_t numberOfSampleInASecond, uint32_t amplitude, uint16_t hold, waveForm wave)
{
	uint16_t freq = note;
	int32_t data[numberOfSampleInASecond]; // data buffer

	uint16_t sampleInAWavePeriod = numberOfSampleInASecond / note; // how many sample in one time period depending by the specific musical note

	uint16_t i = sampleInAWavePeriod / 4;

	switch (wave)
	{
		case sinusoidal:

			for(uint16_t s = 0; s < numberOfSampleInASecond; s++)
			{
				if(s < (sampleInAWavePeriod / 2)) // compute sample for first half-period
				{
					data[s] = ((int32_t)((amplitude) * (sin((2 * pi * s * freq) / (numberOfSampleInASecond)) ) ));
				}
				else if((sampleInAWavePeriod / 2) <= s && (s < sampleInAWavePeriod)) // create sample for second half-period
				{
					data[s] = (- data[s - (sampleInAWavePeriod / 2)]);
				}
				else
				{
					data[s] = (data[s - sampleInAWavePeriod]);
				}
			}

			break;

		case square:

			for(uint16_t s = 0; s < numberOfSampleInASecond; s++)
			{
				if(s < (sampleInAWavePeriod / 2)) // compute sample for first half-period
				{
					data[s] = (amplitude - 5000); // ...to prevent speaker damage...
				}
				else if((sampleInAWavePeriod / 2) <= s && (s < sampleInAWavePeriod)) // create sample for second half-period
				{
					data[s] = (- data[s - (sampleInAWavePeriod / 2)]);
				}
				else
				{
					data[s] = (data[s - sampleInAWavePeriod]);
				}
			}

			break;

		case triangular:

			for(uint16_t s = 0; s < numberOfSampleInASecond; s++)
			{
				if(s < (sampleInAWavePeriod / 4)) // compute sample for first half-period
				{
					data[s] = ((amplitude / (sampleInAWavePeriod / 4)) * s);
				}
				else if(((sampleInAWavePeriod / 4) <= s) && (s < (sampleInAWavePeriod / 2))) // create sample for second half-period
				{
					data[s] =  (data[i - 1]);
					i--;
				}
				else if(((sampleInAWavePeriod / 2) <= s) && (s < sampleInAWavePeriod)) // create sample for second half-period
				{
					data[s] = (- data[s - (sampleInAWavePeriod / 2)]);
				}
				else
				{
					data[s] = (data[s - sampleInAWavePeriod]);
				}
			}

			break;

		case alternate_sawTeeth:
		{
			for(uint16_t s = 0; s < numberOfSampleInASecond; s++)
			{
				if(s < (sampleInAWavePeriod / 2)) // compute sample for first half-period
				{
					data[s] = ((amplitude / (sampleInAWavePeriod / 2)) * s);
				}
				else if((sampleInAWavePeriod / 2) <= s && (s < sampleInAWavePeriod)) // create sample for second half-period
				{
					data[s] = (- data[s - (sampleInAWavePeriod / 2)]);
				}
				else
				{
					data[s] = (data[s - sampleInAWavePeriod]);
				}
			}
		}

		break;
	}

	for(uint16_t i = 0; i < hold; i++)
	{
		UDA1334_sendData(data, data, numberOfSampleInASecond, DSP_OFF, f400Hz); // with DSP_OFF, function parameter filter_cutfreq has no effect.
	}
}


