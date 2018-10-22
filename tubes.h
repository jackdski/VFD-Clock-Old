/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * tubes.h
 */

#include "msp.h"
#include <stdint.h>

/* used for shift register delays */
void enableSystick(uint16_t microseconds);

void disableSystick();

void SysTick_Handler();

// 0-6 for select tubes, 7 for all
void disableOutput(uint8_t target);

void enableOutput(uint8_t target);

void pulseClock();

void configureShiftPins();

uint8_t decToSevSeg(uint8_t value);

void assignPin(uint8_t tube, uint8_t val);

void updateSeconds(uint8_t value);

void updateMinutes(uint8_t value);

void updateHours(uint8_t value);

void updateTime(uint8_t decHrs, uint8_t decMins, uint8_t decSecs);

<<<<<<< HEAD
void shiftOut(uint8_t tubeNumber, uint8_t val);
=======
void shiftOut(uint8_t tubeNumber, uint8_t val);
>>>>>>> e3bd3428fc2b968ce834a8b4b12ab243e27898d4
