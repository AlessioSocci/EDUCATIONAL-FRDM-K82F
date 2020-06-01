/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MK82FN256xxx15_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK82F25615.h"
#include "fsl_debug_console.h"


/* TODO: insert other include files here. */
#include "time.h"

#include <UDA1334ATS.h>
#include <PCM1808.h>
#include "MX25U3235FZNI-10G.h"

/* TODO: insert other definitions and declarations here. */

#define numberOfSampleInASecond_forRXAudioData  44100
#define numberOfSampleInASecond_forTXAudioData  44100

//static uint32_t QSPI_buffer_input[64];
//
//static uint32_t QSPI_buffer_output[64];

/*
 *
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    SIM->SCGC5 |= (1 << 9); // Clock on PORT A
    SIM->SCGC5 |= (1 << 10); // Clock on PORT B
    SIM->SCGC5 |= (1 << 11); // Clock on PORT C
    SIM->SCGC5 |= (1 << 12); // Clock on PORT D
    SIM->SCGC5 |= (1 << 13); // Clock on PORT E

    PORTC->PCR[9] |= (1 << 8); // set GPIO for Green Led
  	GPIOC->PDDR |= (1 << 9);

    // PIT
  	pit_init();

    // UDA1334 (I2S)
    UDA1334_init();

//     PCM1808 (I2S)
    PCM1808_init();

    /* Work in progress */
    // MX25U3235FZNI (QSPI)
//    MX25U3235FZNI_init();
//
//    for(int i = 63; i >= 0; i--)
//    {
//    	QSPI_buffer_input[i] = i;
//    }
//
//    for(int i = 0; i < 64; i++)
//    {
//    	QSPI_buffer_output[i] = 0;
//    }

    static int32_t audioDataL[numberOfSampleInASecond_forRXAudioData];
//    static int32_t audioDataR[numberOfSampleInASecond_forRXAudioData];

    int i = 0;
    /* Enter an infinite loop, just incrementing a counter. */

    while(1)
    {
    	if (i == 0)
    	{
    		UDA1334_sendWaveForm(C4, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(C4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(D4, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(D4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(E4, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(F4, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(F4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(G4, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(G4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(A4, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(A4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(B4, numberOfSampleInASecond_forTXAudioData, 10000, 1, sinusoidal);
    	}

    	if (i == 1)
    	{
    		UDA1334_sendWaveForm(C4, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(C4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(D4, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(D4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(E4, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(F4, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(F4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(G4, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(G4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(A4, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(A4_dies, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    		UDA1334_sendWaveForm(B4, numberOfSampleInASecond_forTXAudioData, 10000, 1, square);
    	}

    	if (arrival_task0_flag == 1) // every 1 s, blink OK status led
    	{
    		uint32_t current_time = ticks;

    		arrival_task0_flag = 0;
    		running_task0_flag = 1;
    	    queued_task0_flag = 0;

    		GPIOC->PTOR |= (1 << 9);

    		running_task0_flag = 0;
    		queuedTime_task0 = 0;
    		burstTime_task0 = ticks - current_time;
    	}

    	if (arrival_task1_flag == 1) // ...nothing to do ... take only few parameters of dummy task
    	{
    		uint32_t current_time = ticks;

    		arrival_task1_flag = 0;
    		running_task1_flag = 1;
    		queued_task1_flag = 0;

    		/* Work in progress */
//     		for (int i = 0; i < FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE; i++) // write all the pages
//    		{
//    			MX25U3235FZNI_program_page(AMBA_BASE_ADDRESS + i * 256, QSPI_buffer_input);
//    		}
//
//    		uint32_t QSPI_dataOut = MX25U3235FZNI_read(QSPI_buffer_output, 32);

    		running_task1_flag = 0;
    		queuedTime_task1 = 0;
    		burstTime_task1 = ticks - current_time;
    	}

    	if (arrival_task2_flag == 1)
    	{
    		uint32_t current_time = ticks;

    		arrival_task2_flag = 0;
    		running_task2_flag = 1;
    		queued_task2_flag = 0;

    		for(int i = 0; i < 100; i++)
    		{
    			PCM1808_receiveWaveForm(audioDataL, audioDataL, numberOfSampleInASecond_forRXAudioData);

    			UDA1334_sendData(audioDataL, audioDataL, numberOfSampleInASecond_forRXAudioData);
    		}

    		running_task2_flag = 0;
    		queuedTime_task2 = 0;
    		burstTime_task2 = ticks - current_time;
    	}

    	i++;
    }
    return 0 ;
}
