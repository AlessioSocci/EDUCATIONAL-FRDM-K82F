/*
 * time.h
 *
 *  Created on: 01 mar 2019
 *      Author: Alessio
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK82F25615.h"
#include "core_cm4.h"

#ifndef TIME_H_
#define TIME_H_

extern uint32_t ticks;

extern uint32_t burstTime_task0;
extern uint32_t burstTime_task1;
extern uint32_t burstTime_task2;


extern uint8_t arrival_task0_flag;
extern uint8_t disable_task0_flag;

extern uint8_t arrival_task1_flag;
extern uint8_t disable_task1_flag;

extern uint8_t arrival_task2_flag;
extern uint8_t disable_task2_flag;


void delay(uint32_t mseconds);
void pit_init(void);

#endif /* TIME_H_ */
