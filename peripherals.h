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
    Clock = 0,         // displays clock
    Setup  = 1,         // set time
    Temperature = 2     // displays temperature
}SwitchMode;

void configure_SystemClock();

void configure_uart();

void configure_buttons();

void configure_leds();

void configure_all_pins();

uint8_t parse_rx_message(CircBuf_t * rxbuf);

#endif
