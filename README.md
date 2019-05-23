# FRDM-K82F

Educational BARE-METAL Project

Test some basic funcitonality for NXP K82F in FRDM board, without libraries.

ATTENTION!! There are many inconsistencies with K82F data sheet to set preemptive priority !! ... this code seems to work well !!

    PIT: interrupt with "high" pre-emptive priority (1), time for delay and state machine, increment internal counter every about 1 ms;
    
    I2S: serial sinc-communication with UDA1334ATS, driver SSD1306. 
    
    MAIN: just once, play the 12 musical note in 4° octave, blink lend every 1 s in task1, nothing to do in task 2.
