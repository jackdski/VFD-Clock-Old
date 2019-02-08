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

// macros for TIMER_A0
#define CLEAR_TIMER (uint16_t)(0x0000)
#define SET_CTL     (uint16_t)(TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_3 | TIMER_A_CTL_MC__UP)
#define COUNT_TO    (uint16_t)(4725) // 47255 for 500ms, 4725 for 50ms
#define SET_CCTL    (uint16_t)(0x0010)

/* timer used for shift register delays */
void enableSystick(uint16_t microseconds);

/* disable the delay timer */
void disableSystick();

/* disable the delay timer after it counts down all the way */
//void SysTick_Handler();

void configure_setup_timer();

/* Will display a time or blanks depending on the input */
//void showTimeSetUp(uint8_t onOff);

#endif /* TIMER_H_ */
