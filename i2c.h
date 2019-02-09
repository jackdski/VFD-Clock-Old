/*
 * VFD Tube Clock
 * Jack Danielski
 *
 *  i2c.h
 */

/* I N C L U D E S */
#include "msp.h"
#include "rtc.h"
#include <stdint.h>
#include "tubes.h"
#include "timer.h"
#include "circbuf.h"

/* D E F I N E S */
/* MPL3115A2 Macros */
#define MPL3115A2           0x60 // 7-bit I2C address
#define STATUS              0x00
#define OUT_T_MSB           0x04
#define OUT_T_LSB           0x05
#define OUT_T_DELTA_MSB     0x0A
#define OUT_T_DELTA_LSB     0x0B
#define WHO_AM_I            0x0C
#define CTRL_REG1           0x26

void configure_i2c();
void write_i2c(uint8_t reg, uint8_t val);
void read_i2c(uint8_t msg, uint8_t bytes);
void write_byte_i2c(uint8_t reg);
void read_byte_i2c(uint8_t reg);
void trigger_sample_mpl();
uint8_t read_temp_c();
uint8_t convert_to_fahrenheit(uint8_t temp_c);
uint8_t read_temp_f();
