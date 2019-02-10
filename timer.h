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

// macros for TIMER_A#
#define CLEAR_TIMER     (uint16_t)(0x0000)
#define SET_CTL         (uint16_t)(TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__8 | TIMER_A_CTL_MC__UP)
#define COUNT_TO_25    (uint16_t)(37500)   // 25ms
#define SET_CCTL        (uint16_t)(0x0010)

///* timer used for shift register delays */
//void enableSystick(uint16_t microseconds);
//
///* disable the delay timer */
//void disableSystick();

/* disable the delay timer after it counts down all the way */
//void SysTick_Handler();

void configure_setup_timer();
void configure_temperature_timer();

#endif /* TIMER_H_ */
