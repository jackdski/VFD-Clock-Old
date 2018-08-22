/*
 *
 * Jack Danielski
 * VFD Tube Clock
 *  rtc.h
 *
 */

#ifndef RTC_H_
#define RTC_H_

#include "msp.h"

typedef enum mode {
    clock = 0,
    setTime = 1,
}Mode;

void configure_rtc();

#endif
