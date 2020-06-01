/*
 * UDA1334.h
 *
 *  Created on: 13 mag 2019
 *      Author: Alessio
 */

#ifndef UDA1334ATS_H_
#define UDA1334ATS_H_

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK82F25615.h"
#include "core_cm4.h"

typedef enum _musicalNote
{
	C2 = 66, C2_dies = 70, D2 = 74, D2_dies = 78, E2 = 83, F2 = 88, F2_dies = 93, G2 = 98, G2_dies = 104, A2 = 110, A2_dies = 117, B2 = 124,

	C3 = 131, C3_dies = 139, D3 = 147, D3_dies = 156, E3 = 165, F3 = 175, F3_dies = 185, G3 = 196, G3_dies = 208, A3 = 220, A3_dies = 233, B3 = 247,

	C4 = 262, C4_dies = 277, D4 = 294, D4_dies = 311, E4 = 330, F4 = 349, F4_dies = 370, G4 = 392, G4_dies = 415, A4 = 440, A4_dies = 466, B4 = 494,

	C5 = 523, C5_dies = 554, D5 = 587, D5_dies = 622, E5 = 659, F5 = 698, F5_dies = 740, G5 = 784, G5_dies = 831, A5 = 880, A5_dies = 932, B5 = 988,

	C6 = 1046, C6_dies = 1109, D6 = 1175, D6_dies = 1245, E6 = 1319, F6 = 1397, F6_dies = 1480, G6 = 1568, G6_dies = 1661, A6 = 1760, A6_dies = 1865, B6 = 1976,

	C7 = 2093, C7_dies = 2217, D7 = 2349, D7_dies = 2489, E7 = 2637, F7 = 2794, F7_dies = 2960, G7 = 3136, G7_dies = 3322, A7 = 3520, A7_dies = 3798, B7 = 3951,

}musicalNote;

typedef enum _waveForm
{
	sinusoidal = 0,
	square = 1,
	triangular = 2,
	alternate_sawTeeth = 3,
}waveForm;


void UDA1334_init(void);
void UDA1334_sendWaveForm(musicalNote note, uint16_t numberOfSampleInASecond, uint32_t amplitude, uint16_t hold, waveForm wave);
void UDA1334_sendData(int32_t* dataAudioL, int32_t* dataAudioR, uint16_t numberOfSampleInASecond);

#endif /* UDA1334ATS_H_ */
