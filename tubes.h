/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * tubes.h
 */

#include "msp.h"
#include <stdint.h>

/* 1-6 for select tubes, 7 for all */
void disableOutput(uint8_t target);

/* Enable the output from the shift register */
void enableOutput(uint8_t target);

/* Pulse clock pin to shift a bit in the shift registers  */
void pulseClock();

/* Configures the shift registers to be used */
void configure_shift_pins();

/* returns a value that will show the input value
 * in the uint8_t on 7-seg display */
uint8_t decToSevSeg(uint8_t value);

/* sets a pin for an assigned tube high or low */
void assignPin(uint8_t tube, uint8_t val);

/* updates only the two tubes related to seconds */
void updateSeconds(uint8_t value);

/* updates only the two tubes related to minutes */
void updateMinutes(uint8_t minsValue, uint8_t secsValue);

/* updates only the two tubes related to seconds */
void updateHours(uint8_t value);

/* updates hours, minutes, and seconds all at once */
void updateTime(uint8_t decHrs, uint8_t decMins, uint8_t decSecs);

/* places a value in the shift register */
void shiftOut(uint8_t tubeNumber, uint8_t val);
