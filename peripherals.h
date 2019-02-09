/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * peripherals.h
 */

#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include "msp.h"
#include "rtc.h"
#include "tubes.h"


void configure_SystemClock();

void configure_uart();

void send_byte(uint8_t data);

void configure_buttons();

void configure_leds();

void configure_all_pins();

#endif
