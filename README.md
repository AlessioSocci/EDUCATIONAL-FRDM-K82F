# FRDM-K82F

Soft Real Time O.S. with Non-Preemptive First Come First Served Task Scheduling, Preemptive Prioritized Interrupt, "Educational" Project - NXP ARM Cortex M4F

Test some basic functionality for NXP K82F in FRDM board, without libraries.

ATTENTION!! There are many inconsistencies with K82F data sheet to set preemptive priority !! ... this code seems to work well !!

    PIT: interrupt with "high" pre-emptive priority (1), time for delay and state machine, increment internal counter every about 1 ms;
    
    I2S: serial sinc-communication with audio decoder UDA1334ATS 24bit audio DAC and with PCM1808 24 bit audio ADC. 
    
    MAIN: just once, play the 12 musical note in 4° octave, blink led every 1 s in task0, and, for now, nothing to do in task 1 (QSPI FLASH...), receive audio data from PCM1808 and send it to UDA1334.
