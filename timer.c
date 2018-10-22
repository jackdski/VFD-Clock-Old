/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * timer.c
 * timer for flashing pattern on startup and changing time
 */

#include "timer.h"

volatile uint8_t activationBit = 0;
volatile uint8_t extraBit = 0;

void TA0_0_IRQHandler() {

    TIMER_A0->CTL &= ~(BIT1);  //Turn off timer interrupts

    /*  */

    //Clear the timer flag
    TIMER_A0->CCTL[0] &= ~(BIT0);
}

void timer_a0_config(){
    TIMER_A0->R = 0;                    // Clear timer count
    TIMER_A0->CTL = SET_CTL;            // Set to SMCLK, Up mode (BIT9 ON)
    TIMER_A0->CCR[0] = COUNT_TO;        // Value to count to
    TIMER_A0->CCTL[0] |= SET_CCTL;      // TACCR0 interrupt enabled
}