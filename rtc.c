/*
 *
 * Jack Danielski
 * VFD Tube Clock
 *  rtc.c
 *
 */

#include "msp.h"
#include "tubes.h"

void configure_rtc() {
//    RTC_C->CTL0 = RTC_C_KEY;
    RTCCTL0_H = RTCKEY_H;
    // Hexadecimal mode, with events going off every hour change
    RTC_C->CTL13 &= ~(RTC_C_CTL13_BCD | RTC_C_CTL13_HOLD);
    RTC_C->CTL13 |= RTC_C_CTL13_MODE | RTC_C_CTL13_TEV_1;

    RTC_C->TIM0 |= RTC_C_TIM0_SEC_MASK;
    RTC_C->CTL0 = 0xFF;
}
