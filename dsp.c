/*
 * dsp.c
 *
 *  Created on: 4 giu 2020
 *      Author: Alessio
 */


#include "dsp.h"
#include "dsp_kernel.h"


void dsp_filter_load(SampleFilter* f, int input, uint8_t tap_number) // setting input signal array and FIR coefficient array
{
  f->history[f->last_index++] = input;

  if(f->last_index >= tap_number)
  {
	  f->last_index = 0;
  }
}

int dsp_filter_process(SampleFilter* f, filter_cutfreq fcut)
{
  int64_t acc = 0;

  int index = f->last_index, i;

  switch(fcut)
  {
  	  case f400Hz: // -24db/oct (.. not properly: from 400 Hz to 800 Hz, attenuation is 24db, after this, gain is approximately constant and doesn't decrease)

  		  for(i = 0; i < TAP_NUMBER_1_LPF; ++i)
		  {
			index = index != 0 ? index - 1 : TAP_NUMBER_1_LPF - 1;

			acc += (int64_t)f->history[index] * dsp_LPF_400Hz_filter_kernel[i]; // for now, without DSP core instruction (MAC)
		  }
  		  break;

  	  case f800Hz: // -24db/oct (.. not properly: from 800 Hz to 1.6 kHz, attenuation is 24db, after this, gain is approximately constant and doesn't decrease)

  		  for(i = 0; i < TAP_NUMBER_2_LPF; ++i)
  		  {
  			  index = index != 0 ? index - 1 : TAP_NUMBER_2_LPF - 1;

  			  acc += (int64_t)f->history[index] * dsp_LPF_800Hz_filter_kernel[i]; // for now, without DSP core instruction (MAC)
  		  }
  		  break;

  	  case f2kHz: // -24db/oct (.. not properly: from 2 kHz to 4 kHz, attenuation is 24db, after this, gain is approximately constant and doesn't decrease)
  		  for(i = 0; i < TAP_NUMBER_3_LPF; ++i)
  		  {
  			index = index != 0 ? index - 1 : TAP_NUMBER_3_LPF - 1;

  			acc += (int64_t)f->history[index] * dsp_LPF_2kHz_filter_kernel[i]; // for now, without DSP core instruction (MAC)
  		  }
  		  break;
  }
  return acc >> 16;
};


