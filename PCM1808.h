/*
 * PCM1808.h
 *
 *  Created on: 30 mag 2019
 *      Author: Alessio
 */

#ifndef PCM1808_H_
#define PCM1808_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK82F25615.h"
#include "core_cm4.h"

#endif /* PCM1808_H_ */


void PCM1808_init(void);
void PCM1808_receiveWaveForm(int32_t* dataAudioL, int32_t* dataAudioR, uint16_t numberOfSampleInASecond);
