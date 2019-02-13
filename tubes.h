/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * tubes.h
 */

#include "msp.h"
#include <stdint.h>

/* 1-6 for select tubes, 7 for all */
void disable_output(uint8_t target);

/* Enable the output from the shift register */
void enable_output(uint8_t target);

/* Pulse clock pin to shift a bit in the shift registers  */
void pulse_clock();

/* Configures the shift registers to be used */
void configure_shift_pins();

/* returns a value that will show the input value
 * in the uint8_t on 7-seg display */
uint8_t dec_to_sev_seg(uint8_t value);

/* sets a pin for an assigned tube high or low */
void assign_pin(uint8_t tube, uint8_t val);

/* updates only the two tubes related to seconds */
void update_seconds(uint8_t value);

/* updates only the two tubes related to minutes */
void update_minutes(uint8_t minsValue, uint8_t secsValue);

/* updates only the two tubes related to seconds */
void update_hours(uint8_t value);

/* updates hours, minutes, and seconds */
void update_time(uint8_t decHrs, uint8_t decMins, uint8_t decSecs);

/* places a value in the shift register */
void update_temperature(uint8_t temperature);

/* places a value in the shift register */
void shift_out(uint8_t tubeNumber, uint8_t val);
