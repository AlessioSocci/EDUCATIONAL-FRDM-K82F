/*
 * dsp.h
 *
 *  Created on: 4 giu 2020
 *      Author: Alessio
 */

#ifndef DSP_H_
#define DSP_H_

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK82F25615.h"
#include "fsl_debug_console.h"
#include "arm_math.h"

#define MAX_TAP_NUMBER_LPF 		105

#define TAP_NUMBER_1_LPF 		105
#define TAP_NUMBER_2_LPF		49
#define TAP_NUMBER_3_LPF		17

typedef enum
{
	LPF,
	HPF,
}filter_type;

typedef enum
{
	f400Hz,
	f800Hz,
	f2kHz,
}filter_cutfreq;

extern int32_t dsp_LPF_400Hz_filter_kernel[105];
extern int32_t dsp_LPF_800Hz_filter_kernel[49];
extern int32_t dsp_LPF_2KHz_filter_kernel[17];

typedef struct
{
	int history[MAX_TAP_NUMBER_LPF];
	unsigned int last_index;
}SampleFilter;


void dsp_filter_load(SampleFilter* f, int input, uint8_t tap_number);

int dsp_filter_process(SampleFilter* f, filter_cutfreq cut);

#endif /* DSP_H_ */
