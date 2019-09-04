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
#include <PCM1803A.h>

/* TODO: insert other definitions and declarations here. */

/*
 *
 * @brief   Application entry point.
 */
int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    SIM->SCGC5 |= (1 << 9); // Clock on PORT A
    SIM->SCGC5 |= (1 << 10); // Clock on PORT B
    SIM->SCGC5 |= (1 << 11); // Clock on PORT C
    SIM->SCGC5 |= (1 << 12); // Clock on PORT D

    PORTC->PCR[9] |= (1 << 8); // set GPIO for Green Led
  	GPIOC->PDDR |= (1 << 9);

    // PIT
  	pit_init();

    // UDA1334 (I2S)
    UDA1334_init();

    // PCM1803A (I2S)
    PCM1803A_init();


    int32_t dataOut[1000];


    int i = 0;
    /* Enter an infinite loop, just incrementing a counter. */

    while(1)
    {
    	if (i == 0)
    	{
    		UDA1334_sendWaveForm(C4, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(C4_dies, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(D4, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(D4_dies, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(E4, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(F4, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(F4_dies, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(G4, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(G4_dies, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(A4, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(A4_dies, 44100, 10000, 1, sinusoidal);
    		UDA1334_sendWaveForm(B4, 44100, 10000, 1, sinusoidal);
    	}

    	if (i == 1)
    	{
    		UDA1334_sendWaveForm(C4, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(C4_dies, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(D4, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(D4_dies, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(E4, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(F4, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(F4_dies, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(G4, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(G4_dies, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(A4, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(A4_dies, 44100, 10000, 1, square);
    		UDA1334_sendWaveForm(B4, 44100, 10000, 1, square);
    	}

    	if (i == 2)
    	{
    		UDA1334_sendWaveForm(C4, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(C4_dies, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(D4, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(D4_dies, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(E4, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(F4, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(F4_dies, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(G4, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(G4_dies, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(A4, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(A4_dies, 44100, 10000, 1, triangular);
    		UDA1334_sendWaveForm(B4, 44100, 10000, 1, triangular);
    	}

    	if (arrival_task0_flag == 1)
    	{
    		uint32_t current_time = ticks;

    		arrival_task0_flag = 0; // every 1 s, blink OK status led

    		GPIOC->PTOR |= (1 << 9);

    		burstTime_task0 = ticks - current_time;
    	}

    	if (arrival_task0_flag == 1)
    	{
    		uint32_t current_time = ticks;

    		arrival_task0_flag = 0;	// ...nothing to do ... except have a burst time of task

    		burstTime_task1 = ticks - current_time;
    	}

    	if (arrival_task2_flag == 1)
    	{
    		uint32_t current_time = ticks;

    		arrival_task2_flag = 0;

    		PCM1803A_receiveWaveForm(dataOut);

    		burstTime_task2 = ticks - current_time;
    	}
    	i++;
    }
    return 0 ;
}
