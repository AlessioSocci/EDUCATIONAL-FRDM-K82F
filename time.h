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

// Time and FSM
extern uint32_t ticks;

extern uint8_t task1Flag;
extern uint8_t task2Flag;

void delay (uint32_t useconds);
void pit_init(void);
#endif /* TIME_H_ */
