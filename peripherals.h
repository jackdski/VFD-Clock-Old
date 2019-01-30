/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * peripherals.h
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include "msp.h"

typedef enum SwitchMode {
    Normal = 0,
    Setup  = 1
}SwitchMode;

void configure_SystemClock();

void configure_uart();

void configure_buttons();

void configure_leds();

void configure_all_pins();

#endif
