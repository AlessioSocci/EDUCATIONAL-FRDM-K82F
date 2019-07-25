/*
 * time.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include "time.h"

// Time and FSM
uint32_t ticks = 0;

uint16_t task1 = 0;
uint16_t task2 = 0;

// BARE METAL O.S. IS A FIFO STATIC - NON-PREEMPTIVE SCHEDULING

uint16_t arrivalTime1 = 1000; // threshold of counter in ms
uint16_t arrivalTime2 = 5000; // threshold of counter in ms

uint8_t task1Flag = 0;
uint8_t task2Flag = 0;


void task1_incrementCounter(void)
{
   	task1++;
	if (task1 == arrivalTime1)
	{
		task1 = 0;
		task1Flag = 1;
	}
}

void task2_incrementCounter(void)
{
   	task2++;
	if (task2 == arrivalTime2)
	{
		task2 = 0;
		task2Flag = 1;
	}
}
void PIT0_IRQHandler(void)
{
	ticks++;
	task1_incrementCounter();
	task2_incrementCounter();
	PIT->CHANNEL->TFLG |= (1 << 0);
}

void delay (uint32_t useconds) // Delay function, based on pit
{
    uint32_t currTicks = ticks;

    while ((ticks - currTicks) < useconds);
}

void pit_init(void)
{
	// PIT
    SIM->SCGC6 |= (1 << 23); // Clock on PIT0

    NVIC->IP[48] = (1 << 4); // Preemptive Priority (1)! (max priority is 0)

    PIT->MCR = 0;

    PIT->CHANNEL->LDVAL = 75000; // 1 ms

    PIT->CHANNEL->TCTRL |= (1 << 0) | (1 << 1);

    NVIC->ISER[1] |= (1 << 16);
}
