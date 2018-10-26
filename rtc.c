/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * rtc.c
 */

#include "msp.h"
#include "tubes.h"

extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;
extern uint8_t timeChanged;

void configure_rtc() {
//    RTC_C->CTL0 = RTC_C_KEY;
    RTCCTL0_H = RTCKEY_H; // use key (A5h) to allow for changes to be made
    RTCCTL0_L |= RTC_C_CTL0_RDYIE; // enable interrupt to allow register reading
    // Hexadecimal mode, with events going off every hour change
    RTC_C->CTL13 &= ~(RTC_C_CTL13_BCD | RTC_C_CTL13_HOLD);
    RTC_C->CTL13 |= RTC_C_CTL13_MODE | RTC_C_CTL13_TEV_1;

    RTC_C->TIM0 |= RTC_C_TIM0_SEC_MASK;
    RTC_C->CTL0 = 0xFF;
    NVIC_EnableIRQ(RTC_C_IRQn);
}

void RTC_C_IRQHandler() {
    if(RTCCTL0_L & RTC_C_CTL0_RDYIFG) { // if correct interrupt
        P2->OUT |= BIT0; // test to see if interrupt happened
        if(RTCCTL13 & RTC_C_CTL13_RDY) { // if safe for reading
            // update values to be loaded
            hours = RTCHOUR;
            minutes = RTCMIN;
            seconds = RTCSEC;
            timeChanged = 1;
        }
    }
    // clear interrupt flag?
}
