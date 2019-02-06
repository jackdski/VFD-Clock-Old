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

void configure_i2c();

void write_i2c(uint8_t reg, uint8_t val);

void read_i2c(uint8_t msg, uint8_t bytes);

void trigger_sample_mpl();

uint8_t read_temp_c();

uint8_t convert_to_fahrenheit(uint8_t temp_c);

uint8_t read_temp_f();

void configure_buttons();

void configure_leds();

void configure_all_pins();

#endif
