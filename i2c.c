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
#define STATUS              0x00
#define OUT_T_MSB           0x04
#define OUT_T_LSB           0x05
#define OUT_T_DELTA_MSB     0x0A
#define OUT_T_DELTA_LSB     0x0B
#define WHO_AM_I            0x0C
#define CTRL_REG1           0x26

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
    P1->SEL0 &= ~(BIT6 | BIT7);
    P1->SEL1 |= (BIT6 | BIT7);

    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_SWRST); // unlock

    // I2C mode, Synchronous mode, Master Mode, SMCLK mode
    EUSCI_B0->CTLW0 = ( EUSCI_B_CTLW0_SWRST | EUSCI_B_CTLW0_MODE_3 | EUSCI_B_CTLW0_SYNC | EUSCI_B_CTLW0_MST | EUSCI_B_CTLW0_UCSSEL_2);

    // disable START condition, do not generate STOP, single master, using 7-bit addy's
    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_TXSTT | EUSCI_B_CTLW0_TXSTP | EUSCI_B_CTLW0_MM | EUSCI_B_CTLW0_SLA10 | EUSCI_B_CTLW0_A10);

    EUSCI_B0->I2CSA = MPL3115A2;  // load MPL3115A2's address
    EUSCI_B0->BRW = 0x001E;  // SMCLK /30 <= 400kHz

    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);  // lock

    EUSCI_B0->IE = (EUSCI_B_IE_RXIE | EUSCI_B_IE_TXIE | EUSCI_B_IE_NACKIE | EUSCI_B_IE_STPIE); // enable RX, TX, and NACK interrupts
    EUSCI_B0->IFG = 0; // clear flags
//    NVIC_EnableIRQ(EUSCIB0_IRQn);
    NVIC->ISER[0] = 1 << ((EUSCIB0_IRQn) & 31);
}

//void write_i2c(uint8_t reg, uint8_t val) {
//    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;        // set to write/transmitter
//    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;     // send start and addy
//    EUSCI_B0->TXBUF = reg;
//    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);
//    EUSCI_B0->TXBUF = val;
//    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;     // send stop
//}
//
//void read_i2c(uint8_t msg, uint8_t bytes) {
//    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;        // set to write/transmitter
//    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;     // send start and addy
//    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);
//    EUSCI_B0->TXBUF = msg;
//    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_TR);     // set to receiver
//    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;     // send restart
//    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);// block?
//    while(EUSCI_B0->STATW & EUSCI_B_STATW_BCNT_MASK != (bytes << 8)); // read specified amount of bytes
//    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;          // send stop
//}

void write_byte_i2c(uint8_t reg) {
    uint16_t txieStatus = EUSCI_B0->IE & UCTXIE; // store current TXIE status
    EUSCI_B0->IE &= ~(UCTXIE_OFS);
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;        // set to write
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;     // send start and addy
    EUSCI_B0->IFG |= UCTXIFG;
    while(EUSCI_B0->IFG & UCTXIFG);
//    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT);
    EUSCI_B0->TXBUF = reg;     // write to reg
    while(EUSCI_B0->IFG & UCTXIFG);
    EUSCI_B0->IFG |= UCTXIFG;
    while(EUSCI_B0->IFG & UCTXIFG);
//    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
//    EUSCI_B0->TXBUF = value;   // write to reg
//    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;  // send stop
    EUSCI_B0->IFG &= ~UCTXIFG;
    EUSCI_B0->IE |= txieStatus;
}

void read_byte_i2c(uint8_t reg){
    addItemCircBuf(TXBuf_i2c, reg);
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;        // set to write
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;     // send start and addy
    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT); // wait for start and addy to send
//    EUSCI_B0->TXBUF = reg;                      // send data
//    while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0));  // wait it to be done sending
    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_TR);     // set to receiver
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;     // send restart
//    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTT); // wait for restart and addy to send
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;          // send stop
    while(RXBuf_i2c->num_items == 0);           // wait for byte back
//    while(EUSCI_B0->CTLW0 & EUSCI_B_CTLW0_TXSTP); // wait for stop
}

// triggers a sample on the temperature sensor
void trigger_sample_mpl() {
    // set OST low
    read_byte_i2c(CTRL_REG1);
    uint8_t setting = removeItem(RXBuf_i2c);
    setting &= ~(1 << 1);                   // clear OST bit
//    write_byte_i2c(CTRL_REG1, setting);     // send

    // set OST high again
    read_byte_i2c(CTRL_REG1);
    setting = removeItem(RXBuf_i2c);
    setting |= (1 << 1);        // set OST bit
//    write_byte_i2c(CTRL_REG1, setting);
}

uint8_t read_temp_c() {
    trigger_sample_mpl(); // trigger a new temp sample
    uint8_t status = 0;
    uint8_t temp_c;

    // wait for new temp data
    while(status & (1 << 1) == 0) {
        read_byte_i2c(STATUS);
        status = removeItem(RXBuf_i2c);
    }

    // get temp data
    read_byte_i2c(OUT_T_MSB);
    temp_c = removeItem(RXBuf_i2c);
    return temp_c;
}

uint8_t convert_to_fahrenheit(uint8_t temp_c) {
    return (temp_c * 9) / 5 + 32;
}

uint8_t read_temp_f(){
    P1->OUT ^= BIT0;
    uint8_t final_temp = convert_to_fahrenheit(read_temp_c());
    if(final_temp > 65) {
        P2->OUT ^= BIT1;
    }
    return final_temp;
}

void set_mode_standby() {
    read_byte_i2c(CTRL_REG1);
    uint8_t setting = removeItem(RXBuf_i2c);
    setting &= ~(1 << 0);  // clear standby bit
//    write_byte_i2c(CTRL_REG1, setting);
}

void set_mode_active() {
    read_byte_i2c(CTRL_REG1);
    uint8_t setting = removeItem(RXBuf_i2c);
    setting |= (1 << 0);  // clear standby bit
//    write_byte_i2c(CTRL_REG1, setting);
}

// I2C interrupts
void EUSCIB0_IRQHandler() {
    // NACK received
    if(EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG) {
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_NACKIFG);  // clear flag
//        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT; // send start message
    }

    if(EUSCI_B0->IFG & EUSCI_B_IFG_STPIFG) {
        EUSCI_B0->IFG &= ~EUSCI_B_IFG_STPIFG;
        tx_size = 0;
    }

    // transmitting
    if(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0) {
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_TXIFG0);  // clear flag
//        if(!isEmpty(TXBuf_i2c)) {
//            EUSCI_B0->TXBUF = removeItem(TXBuf_i2c);
//        }
//        else {
////            EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
////            EUSCI_B0->IFG &= ~EUSCI_B_IFG_TXIFG;
//            EUSCI_B0->TXBUF = 0; // load dummy byte
//        }
//        tx_size++;
    }

    // receiving
    if(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0) {
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_RXIFG0);  // clear flag
        addItemCircBuf(RXBuf_i2c, EUSCI_B0->RXBUF);  // throw in RX circbuf
//        if(rx_size < 5) {
//            RXData[rx_size] = EUSCI_B0->RXBUF;
//            rx_size++;
//        }
//        if(rx_size > 5) {
//            rx_size = 0;
//        }
    }
}
