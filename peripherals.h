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
    Setup  = 1,
    Temperature = 2,
    Off = 3
}SwitchMode;

void configure_SystemClock();

void configure_uart();

void configure_buttons();

void configure_relay();

void set_relay_on();

void set_relay_off();

void configure_leds();

void configure_all_pins();

#endif
