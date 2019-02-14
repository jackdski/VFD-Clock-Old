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
#include "circbuf.h"

void configure_SystemClock();

void configure_uart();

void configure_buttons();

void configure_leds();

void configure_all_pins();

uint8_t parse_rx_message(CircBuf_t * rxbuf);

#endif
