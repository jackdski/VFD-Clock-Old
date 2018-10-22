/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * timer.c
 * timer for flashing pattern on startup and changing time
 */

#include <stdint.h>

#include "msp.h"
#include "timer.h"
#include "tubes.h"

//extern Mode state;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;

extern uint8_t onOff; // 0 = blanks, 1 = current time
extern uint8_t setupChanged;

void TA0_0_IRQHandler() {

    TIMER_A0->CTL &= ~(BIT1);  //Turn off timer interrupts

    if( onOff )
        onOff = 0;
    if( !onOff )
        onOff = 1;

    setupChanged = 1;

    //Clear the timer flag
    TIMER_A0->CCTL[0] &= ~(BIT0);
}

void timer_a0_config(){
    TIMER_A0->R = 0;                    // Clear timer count
    TIMER_A0->CTL = SET_CTL;            // Set to SMCLK, Up mode (BIT9 ON)
    TIMER_A0->CCR[0] = COUNT_TO;        // Value to count to
    TIMER_A0->CCTL[0] |= SET_CCTL;      // TACCR0 interrupt enabled
}

/* Will display a time or blanks depending on the input */
void showTimeSetUp(uint8_t onOff) {
    if( onOff == 1 ) {
        // display current time
        updateTime(hours, minutes, seconds);
    }
    if( onOff == 0 ) {
        // dislay blanks
        updateTime(0, 0, 0);
    }
}
