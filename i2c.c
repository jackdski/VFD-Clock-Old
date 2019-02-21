/*
 * VFD Tube Clock
 * Jack Danielski
 *
 *  i2c.c
 */

/* I N C L U D E S */
#include "msp.h"
#include "rtc.h"
#include <stdint.h>
#include "tubes.h"
#include "timer.h"
#include "circbuf.h"
#include "i2c.h"

/* D E F I N E S */
/* MPL3115A2 Macros */
#define MPL3115A2           0x60 // 7-bit I2C address
//#define MPL3115A2           0xC0
#define STATUS              0x00
#define OUT_T_MSB           0x04
#define OUT_T_LSB           0x05
#define OUT_T_DELTA_MSB     0x0A
#define OUT_T_DELTA_LSB     0x0B
#define WHO_AM_I            0x0C
#define CTRL_REG1           0x26
#define PT_DATA_CFG         0x13
#define ACTIVE              0xB9  //value to write


/* G L O B A L   V A R I A B L E S */
//extern Mode state;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;

extern uint8_t doButtons;
extern uint8_t buttonCount;

extern CircBuf_t * RXBuf;
extern CircBuf_t * TXBuf;

extern CircBuf_t * RXBuf_i2c;
extern CircBuf_t * TXBuf_i2c;

uint8_t TXData[5];
//uint8_t RXData[5];
uint8_t tx_size = 0;
//uint8_t rx_size = 0;


void configure_i2c(){
    /*
     * UCB0SDA -> P1.6
     * UCB0SCL -> P1.7
     */

    // configure GPIO
//    P1->SEL0 &= ~(BIT6 | BIT7);
//    P1->SEL1 |= (BIT6 | BIT7);
    P1->SEL0 |= BIT6 | BIT7;                // I2C pins

    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_SWRST); // unlock

    // I2C mode, Synchronous mode, Master Mode, SMCLK mode
    EUSCI_B0->CTLW0 = ( EUSCI_B_CTLW0_SWRST);
    EUSCI_B0->CTLW0 = ( EUSCI_B_CTLW0_SWRST |
            EUSCI_B_CTLW0_MODE_3 |
            EUSCI_B_CTLW0_SYNC |
            EUSCI_B_CTLW0_MST |
            EUSCI_B_CTLW0_SSEL__SMCLK);

    // disable START condition, do not generate STOP,
    //  single master, using 7-bit addy's
//    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_TXSTT |
//            EUSCI_B_CTLW0_TXSTP |
//            EUSCI_B_CTLW0_MM |
//            EUSCI_B_CTLW0_SLA10 |
//            EUSCI_B_CTLW0_A10);

    EUSCI_B0->I2CSA = MPL3115A2;  // load MPL3115A2's address
    EUSCI_B0->BRW = 30;  // SMCLK /30 <= 400kHz

    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);  // lock

    // enable RX, TX, and NACK interrupts
    EUSCI_B0->IE = (EUSCI_B_IE_RXIE |
            EUSCI_B_IE_TXIE |
            EUSCI_B_IE_NACKIE);

    EUSCI_B0->IFG = 0; // clear flags
//    NVIC_EnableIRQ(EUSCIB0_IRQn);
    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);
}


void writeRegister(uint8_t reg, uint8_t value){
//    while(UCB0STATW & UCBBUSY);     // wait while busy
//    while(UCB0CTLW0 & UCTXSTT);     // send start
    UCB0CTL0 |= UCTR | UCTXSTT;     // send start and addy
    while(UCB0CTLW0 & UCTXSTT);     // wait for start and addy to send
    UCB0TXBUF = reg;                // send reg to be written to
    while(!(UCB0IFG & UCTXIFG0));   // wait to send reg
    UCB0TXBUF = value;              // send value to reg
    while(!(UCB0IFG & UCTXIFG0));   //  wait for value to be sent
    UCB0CTL0 |= UCTXSTP;            // send stop
//    while(UCB0CTLW0 & UCTXSTP);     // wait for stop to be sent
}


uint8_t readRegister(uint8_t reg){
    UCB0CTL0 |= UCTR | UCTXSTT;     // set to transmitter, send start and addy
    while(UCB0CTLW0 & UCTXSTT);     // wait for start and addy to send
    UCB0TXBUF = reg;                // send the reg to be read
    while(!(UCB0IFG & UCTXIFG0));   // wait to be transmitted
    UCB0CTL0 &= ~UCTR;              // set to receiver
    UCB0CTL0 |= UCTXSTT;            // send restart
    while(UCB0CTLW0 & UCTXSTT);     // wait for restart
    //  while(!(UCB0IFG & UCRXIFG0));
    UCB0CTL0 |= UCTXSTP;            // send stop
    uint8_t rxValue = UCB0RXBUF;    // read received byte
    while(UCB0CTLW0 & UCTXSTP);     // wait for stop to be sent
    return rxValue;                 // return reg's value
}


// triggers a sample on the temperature sensor
void trigger_sample_mpl() {
    // get and set OST low
    uint8_t setting = readRegister(CTRL_REG1);
    setting &= ~(1 << 1);                   // clear OST bit
    writeRegister(CTRL_REG1, setting);

    // set OST high again
    setting = readRegister(CTRL_REG1);
    setting |= (1 << 1);        // set OST bit
}


uint8_t read_temp_c() {
    trigger_sample_mpl(); // trigger a new temp sample
    uint8_t status = 0;
    uint8_t temp_c;

    // wait for new temp data
    while(status & 0x08 == 0) {
        status = readRegister(OUT_T_MSB);
    }
    temp_c = readRegister(OUT_T_MSB);
    return temp_c;
}


uint8_t convert_to_fahrenheit(uint8_t temp_c) {
    return (temp_c * 9) / 5 + 32;
}


uint8_t read_temp_f(){
    P1->OUT ^= BIT0;
    uint8_t final_temp = convert_to_fahrenheit(read_temp_c());
    if(final_temp > 65) {       // blink if warm out
        P2->OUT ^= BIT1;
    }
    return final_temp;
}

void set_mode_standby() {
    uint8_t setting = readRegister(CTRL_REG1);  // read setting
    setting &= ~(1 << 0);                       // set standby bit
    writeRegister(CTRL_REG1, setting);          // writ to reg
}

void set_mode_active() {
    uint8_t setting = readRegister(CTRL_REG1);  // read setting
    setting |= (1 << 0);                        // set active bit
    writeRegister(CTRL_REG1, setting);          // write to reg
}


void set_oversample_rate(uint8_t sample_rate) {
  if(sample_rate > 7) sample_rate = 7;
  sample_rate <<= 3;
  uint8_t setting = readRegister(CTRL_REG1);    // read setting
  setting &= 0b11000111;                    // clear out OS bits
  setting |= sample_rate;                   // use new OS bits
  writeRegister(CTRL_REG1, setting);        // write to reg
}


void enableEventFlags() {
  writeRegister(PT_DATA_CFG, 0x07); // Enable all three pressure and temp event flags
}


// I2C interrupts
void EUSCIB0_IRQHandler() {
    // NACK received
    if(EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG) {
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_NACKIFG);  // clear flag
    }

    if(EUSCI_B0->IFG & EUSCI_B_IFG_STPIFG) {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_STPIFG;
        tx_size = 0;
    }

    // transmitting
    if(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0) {
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_TXIFG0);  // clear flag
    }

    // receiving
    if(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0) {
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_RXIFG0);  // clear flag
    }
}
