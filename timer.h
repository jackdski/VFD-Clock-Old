/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * timer.h
 * timer for flashing pattern on startup and changing time
 *
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "msp.h"
#include <stdint.h>
#include "tubes.h"

#define CLEAR_TIMER (uint16_t)(0x0000)
#define SET_CTL     (uint16_t)(TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_3 | TIMER_A_CTL_MC__UP)
#define COUNT_TO    (uint16_t)(65000) // 47255 for 500ms on time
#define SET_CCTL    (uint16_t)(0x0010)

void configure_setup_timer();

/* Will display a time or blanks depending on the input */
void showTimeSetUp(uint8_t onOff);

#endif /* TIMER_H_ */
