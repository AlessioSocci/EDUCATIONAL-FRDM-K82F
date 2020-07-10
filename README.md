# FRDM-K82F

Soft Real Time O.S. with Non-Preemptive First Come First Served Task Scheduling, Preemptive Prioritized Interrupt, "Educational" Project - NXP ARM Cortex M4F

Test some basic functionality for NXP K82F in FRDM board, without libraries.

ATTENTION !! There are many inconsistencies with K82F data sheet, to set preemptive priority. This code works.


PIT: interrupt with "high" pre-emptive priority (1), time for delay and state machine, increment internal counter every about 1 ms;
    
I2S: serial sync-communication with with PCM1808 24 bit - 44100 Hz (96 kHz is also possible...) audio ADC and UDA1334ATS 24bit audio 
     DAC connected to a small power integrated audio stereo amplifier with 2 nearly full-range speaker at it's end (really range is 
     approximatively flat from 200 Hz to 4 KHz... but is good enough to prove this example);
    
SPI: serial sync-communication with external 8 MB - 24bit addressed, flash memory.
    
MAIN: just once, play the 12 musical note in 4° octave in sinusoidal and square waveform, test some transaction to external flash memory,
      blink led every 1 s in task0, and, for now, nothing to do in task 1 (working on QSPI FLASH), read previous memorized input audio data to externl SPI    
      flash memory. 
      In task3, receive audio data from PCM1808 ADC and send it to UDA1334 DAC, first time in flat, after appling FIR low pass filter 
      in sequence of: 400Hz - 800 Hz - 2 kHz, another reading of audio input and memorize it in external flash memory. 

