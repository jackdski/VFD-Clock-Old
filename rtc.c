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
//extern uint8_t timeChanged;

void configure_rtc() {
//    RTC_C->CTL0 = RTC_C_KEY;
    RTCCTL0_H = RTCKEY_H; // use key (A5h) to allow for changes to be made
    RTCCTL0_L |= RTC_C_CTL0_RDYIE; // enable interrupt to allow register reading
    // Hexadecimal mode, with events going off every hour change
    RTC_C->CTL13 &= ~(RTC_C_CTL13_BCD | RTC_C_CTL13_HOLD);
    RTC_C->CTL13 |= RTC_C_CTL13_MODE | RTC_C_CTL13_TEV_1;

    // enable interrupts at 1Hz
    RTC_C->PS1CTL |= (RTC_C_PS1CTL_RT1IP__128 | RTC_C_PS1CTL_RT1PSIE); // enable interrupts

    RTC_C->TIM0 |= RTC_C_TIM0_SEC_MASK;
    RTC_C->CTL0 = 0xFF;
    NVIC_EnableIRQ(RTC_C_IRQn);
}

void RTC_C_IRQHandler() {
    // if ready to read from
//    if(RTCCTL0_L & RTC_C_CTL0_RDYIFG) { // if correct interrupt
//        P2->OUT |= BIT0; // test to see if interrupt happened
//        if(RTCCTL13 & RTC_C_CTL13_RDY) { // if safe for reading
//            // update values to be loaded
//            hours = RTCHOUR;
//            minutes = RTCMIN;
//            seconds = RTCSEC;
//            timeChanged = 1;
//        }
        // update time
//    }

    uint8_t changes = 0;

    // second was changed
    if (RTC_C->PS1CTL & RTC_C_PS1CTL_RT1PSIFG) {
        RTC_C->PS1CTL & ~RTC_C_PS1CTL_RT1PSIFG; // clear flag
        changes |= 0b001;
        P2->OUT ^= BIT2;
    }

    // minute was changed
    if(RTCCTL13_L & RTC_C_CTL13_TEV_0)
        changes |= 0b011;

    // hour was changed
    if(RTCCTL13_L & RTC_C_CTL13_TEV_1)
        changes |= 0b111;

    // if able to read update tubes
    if(RTCCTL0_L & RTC_C_CTL0_RDYIFG) {
        // now update tubes
        switch(changes) {
        case 0b001:
            updateSeconds(RTCSEC);
            break;
        case 0b011:
            updateMinutes(RTCMIN, RTCSEC);
            break;
        case 0b111:
            updateTime(RTCHOUR, RTCMIN, RTCSEC);
            break;
        }
    }

//    RTCCTL0_L &=  !RTC_C_CTL0_RDYIFG;// clear interrupt flag?
    RTCIV &= 0x00; // clear interrupt
}
