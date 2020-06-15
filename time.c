/*
 * time.c
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include "time.h"

uint32_t ticks = 0;

uint16_t arrival_task0_counter = 0;
uint16_t revival_task0_counter = 0;
uint32_t burstTime_task0 = 0;
uint32_t queuedTime_task0 = 0;

uint16_t arrival_task1_counter = 0;
uint16_t revival_task1_counter = 0;
uint32_t burstTime_task1 = 0;
uint32_t queuedTime_task1 = 0;

uint16_t arrival_task2_counter = 0;
uint16_t revival_task2_counter = 0;
uint32_t burstTime_task2 = 0;
uint32_t queuedTime_task2 = 0;

uint16_t arrival_task3_counter = 0;
uint16_t revival_task3_counter = 0;
uint32_t burstTime_task3 = 0;
uint32_t queuedTime_task3 = 0;

// BARE METAL O.S. IS A FIFO STATIC - NON-PREEMPTIVE SCHEDULING

uint32_t currTicks = 0;

uint32_t arrival_task0_time = 1000; // threshold of counter in ms
uint32_t revival_task0_time = 5000;

uint32_t arrival_task1_time = 2500;
uint32_t revival_task1_time = 5000;

uint32_t arrival_task2_time = 4100;
uint32_t revival_task2_time = 5000;

uint32_t arrival_task3_time = 6200;
uint32_t revival_task3_time = 5000;

uint8_t arrival_task0_flag = 0;
uint8_t disable_task0_flag = 0;
uint8_t running_task0_flag = 0;
uint8_t queued_task0_flag = 0;

uint8_t arrival_task1_flag = 0;
uint8_t disable_task1_flag = 0;
uint8_t running_task1_flag = 0;
uint8_t queued_task1_flag = 0;

uint8_t arrival_task2_flag = 0;
uint8_t disable_task2_flag = 0;
uint8_t running_task2_flag = 0;
uint8_t queued_task2_flag = 0;

uint8_t arrival_task3_flag = 0;
uint8_t disable_task3_flag = 0;
uint8_t running_task3_flag = 0;
uint8_t queued_task3_flag = 0;

void task_scheduler(void)
{
	arrival_task0_counter++;
	if(arrival_task0_counter == arrival_task0_time)
	{
		arrival_task0_counter = 0;
		arrival_task0_flag = 1;

		if(running_task1_flag == 1 || running_task2_flag == 1 || running_task3_flag == 1)
		{
			queued_task0_flag = 1;
		}
	}

	revival_task0_counter++;
	if(revival_task0_counter == revival_task0_time && disable_task0_flag == 1)
	{
		revival_task0_counter = 0;
		arrival_task0_counter = 0;
		disable_task0_flag = 0;
	}
	else if(revival_task0_counter == revival_task0_time && disable_task0_flag == 0)
	{
		revival_task0_counter = 0;
	}


	arrival_task1_counter++;
	if(arrival_task1_counter == arrival_task1_time)
	{
		arrival_task1_counter = 0;
		arrival_task1_flag = 1;

		if(running_task0_flag == 1 || running_task2_flag == 1 || running_task3_flag == 1)
		{
			queued_task1_flag = 1;
		}
	}

	revival_task1_counter++;
	if(revival_task1_counter == revival_task1_time && disable_task1_flag == 1)
	{
		revival_task1_counter = 0;
		arrival_task1_counter = 0;
		disable_task1_flag = 0;
	}
	else if(revival_task1_counter == revival_task1_time && disable_task1_flag == 0)
	{
		revival_task1_counter = 0;
	}


	arrival_task2_counter++;
	if(arrival_task2_counter == arrival_task2_time)
	{
		arrival_task2_counter = 0;
		arrival_task2_flag = 1;

		if(running_task0_flag == 1 || running_task1_flag == 1 || running_task3_flag == 1)
		{
			queued_task2_flag = 1;
		}
	}

	revival_task2_counter++;
	if(revival_task2_counter == revival_task2_time && disable_task2_flag == 1)
	{
		revival_task2_counter = 0;
		arrival_task2_counter++;
		disable_task2_flag = 0;
	}
	else if(revival_task2_counter == revival_task2_time && disable_task2_flag == 0)
	{
		revival_task2_counter = 0;
	}


	arrival_task3_counter++;
	if(arrival_task3_counter == arrival_task3_time)
	{
		arrival_task3_counter = 0;
		arrival_task3_flag = 1;

		if(running_task0_flag == 1 || running_task1_flag == 1  || running_task2_flag == 1)
		{
			queued_task3_flag = 1;
		}
	}

	revival_task3_counter++;
	if(revival_task3_counter == revival_task3_time && disable_task3_flag == 1)
	{
		revival_task3_counter = 0;
		arrival_task3_counter++;
		disable_task3_flag = 0;
	}
	else if(revival_task3_counter == revival_task3_time && disable_task3_flag == 0)
	{
		revival_task3_counter = 0;
	}


	if(queued_task0_flag == 1)
	{
		queuedTime_task0++;
	}

	if(queued_task1_flag == 1)
	{
		queuedTime_task1++;
	}

	if(queued_task2_flag == 1)
	{
		queuedTime_task2++;
	}

	if(queued_task3_flag == 1)
	{
		queuedTime_task3++;
	}

}

void PIT0_IRQHandler(void)
{
	ticks++;

	task_scheduler();

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
