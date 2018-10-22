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

#define CLEAR_TIMER (uint16_t)(0x0000)
#define SET_CTL     (uint16_t)(TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_ID_3 | TIMER_A_CTL_MC__UP)
#define COUNT_TO    (uint16_t)(37799) //Value for 5Hz is 37499 by experiment  47255 for 500ms on time
#define SET_CCTL    (uint16_t)(0x0010)

void timer_a0_config();

<<<<<<< HEAD
#endif /* TIMER_H_ */
=======
#endif /* TIMER_H_ */
>>>>>>> e3bd3428fc2b968ce834a8b4b12ab243e27898d4
