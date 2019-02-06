/*
 * VFD Tube Clock
 * Jack Danielski
 *
 *  peripherals.c
 */

#include "msp.h"
#include "rtc.h"
#include <stdint.h>
#include "tubes.h"
#include "timer.h"
#include "circbuf.h"

/*
 * MPL3115A2 Macros
 */
#define MPL3115A2           0x60 // 7-bit I2C address
#define STATUS              0x00
#define OUT_T_MSB           0x04
#define OUT_T_LSB           0x05
#define OUT_T_DELTA_MSB     0x0A
#define OUT_T_DELTA_LSB     0x0B
#define WHO_AM_I            0x0C
#define CTRL_REG1           0x26


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

void configure_SystemClock(){
    CS-> KEY = 0x695A; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.
}

// configure on P1.2 (RX) and P1.3 (TX) to be used for
//  Bluetooth UART communication
void configure_uart(){
    //Configure UART pins, set 2-UART pins to UART mode
    P1->SEL0 |=  (BIT2 | BIT3);
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
    EUSCI_A0->CTLW0 |= (BIT7);                  //Select Frame parameters and source
    EUSCI_A0->BRW = 78;                         //Set Baud Rate
    EUSCI_A0->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI

    EUSCI_A0->IFG &= ~(BIT1 | BIT0);
    UCA0IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    NVIC_EnableIRQ(EUSCIA0_IRQn);
}

void send_byte(uint8_t data) {
/*
:param: uint8_t data: byte to be sent over UART
 */
    EUSCI_A0->TXBUF = data;
}

void configure_i2c(){
    /*
     * UCB0SDA -> P1.6
     * UCB0SCL -> P1.7
     *
     * MPL3115A2:
     *  - I2C Address:  60h
     *  - 8-bit read:   C1h
     *  - 8-bit write:  C0h
     */

    P1->SEL0 |= (BIT6 | BIT7);

    // enable reset, I2C, SYNC, Master Mode, and SMCLK
    EUSCI_B0->CTLW0 |= (EUSCI_B_CTLW0_SWRST | EUSCI_B_CTLW0_MODE_3 |
            EUSCI_B_CTLW0_SYNC| EUSCI_B_CTLW0_MST) | EUSCI_B_CTLW0_UCSSEL_2;

    // disable START condition, do not generate STOP, single master, using 7-bit addy's
    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_TXSTT | EUSCI_B_CTLW0_TXSTP |
            EUSCI_B_CTLW0_MM | EUSCI_B_CTLW0_SLA10 | EUSCI_B_CTLW0_A10);

    EUSCI_B0->I2CSA = 0x60; // MPL3115A2's address
    EUSCI_B0->BRW = 0x0014; // SMCLK /20 = 400k
    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);

    // enable RX, TX, and NACK interrupts
    EUSCI_B0->IE |= (EUSCI_B_IE_RXIE0 | EUSCI_B_IE_TXIE0 | EUSCI_B_IE_NACKIE);
    EUSCI_B0->IFG = 0; // clear flags
    NVIC_EnableIRQ(EUSCIB0_IRQn);
}

void write_i2c(uint8_t reg, uint8_t val) {
    addItemCircBuf(TXBuf_i2c, reg);
    addItemCircBuf(TXBuf_i2c, val);
//    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;     // enable
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TR;        // set to write/transmitter
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;     // send start and addy
    EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG0;        // send
    // block?
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;     // send stop
//    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);  // disable
}

void read_i2c(uint8_t msg, uint8_t bytes) {
//    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;     // enable
    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_TR);     // set to receiver
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;     // send start and addy
    addItemCircBuf(TXBuf_i2c, msg);             // load msg
    EUSCI_B0->IFG |= EUSCI_B_IFG_TXIFG0;        // send msg
    // block?
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT;     // send restart
    while(EUSCI_B0->STATW & EUSCI_B_STATW_BCNT_MASK != (bytes << 8)); // read specified amount of bytes
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP;          // send stop
//    EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);  // disable
}

// triggers a sample on the temperature sensor
void trigger_sample_mpl() {
    // set OST low
    read_i2c(CTRL_REG1, 1);
    while(isEmpty(RXBuf_i2c)); // wait for value back
    uint8_t setting = removeItem(RXBuf_i2c);
    setting &= ~(1 << 1); // clear OST bit
    write_i2c(CTRL_REG1, setting);

    // set OST high again
    read_i2c(CTRL_REG1, 1);
    while(isEmpty(RXBuf_i2c)); // wait for value back
    setting = removeItem(RXBuf_i2c);
    setting |= (1 << 1);        // set OST bit
    write_i2c(CTRL_REG1, setting);
}

uint8_t read_temp_c() {
    trigger_sample_mpl();
    read_i2c(STATUS, 1);
    while(isEmpty(RXBuf_i2c)); // wait for value back
    if(removeItem(RXBuf_i2c) & (1 << 1) == 0) {
        return 0;
    }
    read_i2c(OUT_T_MSB, 2);
    while(RXBuf_i2c->num_items < 2); // wait to get two bytes from MPL
    uint8_t lsb_temp = removeItem(RXBuf_i2c);
    uint8_t msb_temp = removeItem(RXBuf_i2c);
    // convert
    lsb_temp = (lsb_temp >> 4) / 16;
    return (msb_temp + lsb_temp);
}

uint8_t convert_to_fahrenheit(uint8_t temp_c) {
    return (temp_c * 9) / 5 + 32;
}

uint8_t read_temp_f(){
    P1->OUT ^= BIT0;
    return convert_to_fahrenheit(read_temp_c());
}

void configure_buttons() {
    // P5.0 -> Change Time Switch
    // P5.1 -> '+' Button
    // P5.2 -> '-' Button

    // config switch and button SEL reg's
    P5->SEL0 &= ~(BIT0 | BIT1 | BIT2);
    P5->SEL1 &= ~(BIT0 | BIT1 | BIT2);

    // config switch and +/- buttons direction and pulldown resistor
    P5->DIR &= ~(BIT0 | BIT1 | BIT4 | BIT5);  // put to input direction
    P5->REN |= (BIT0 | BIT1 | BIT4 | BIT5);    // enable pullup/down resistor
    P5->OUT &= ~(BIT0 | BIT1 | BIT4 | BIT5);   // enable pulldown resistor
    P5->IE |= (BIT1 | BIT2);

    NVIC_EnableIRQ(PORT5_IRQn);
}

void configure_leds() {
    // config LED
    P1->SEL0 &= ~(BIT0);
    P1->SEL1 &= ~(BIT0);
    P1->DIR |=  (BIT0);
    P1->OUT &= ~(BIT0);

    // Config Port 2 LED for debugging help
    P2->SEL0 &= ~(BIT0 | BIT1 | BIT2);
    P2->SEL1 &= ~(BIT0 | BIT1 | BIT2);
    P2->DIR |= (BIT0 | BIT1 | BIT2);
    P2->OUT &= ~(BIT0 | BIT1 | BIT2);
}

/* set all unused pins low */
void configure_all_pins() {
    P1->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P1->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P2->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P2->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P3->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P4->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P4->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P5->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P5->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P6->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P6->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P7->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P7->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P8->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P8->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P9->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P9->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P10->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    P10->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
}

// +/- control
void PORT5_IRQHandler() {
    // "+" Button
    if(P5->IFG & BIT1 && P5->IN & BIT0) {
        // if rising edge
        if(P5->IES & BIT1) {
            P5->IES &= ~BIT1; // set to falling edge
            doButtons = 0b01;
            enableSystick(50); // set to 50ms
        }

        // if falling edge
        else if(P5->IES & ~BIT1) {
            P5->IES &= BIT1; // set to rising edge
            doButtons = 0b10;
            disableSystick();
            RTCSEC = 0;
            buttonCount = 0; // reset button count
        }
    }

    // "-" Button
    if(P5->IFG & BIT2 && P5->IN & BIT0) {
        // if rising edge
        if(P5->IES & BIT2) {
            P5->IES &= ~BIT2; // set to falling edge
            doButtons = 0b00010000;
            enableSystick(50); // set to 50ms
        }

        // if falling edge
        else if(P5->IES & ~BIT2) {
            P5->IES &= BIT2; // set to rising edge
            doButtons = 0b10010000;
            disableSystick();
            RTCSEC = 0;
            buttonCount = 0; // reset button count
        }
    }
    P5->IFG = 0; // clear interrupt flags
}

// UART interrupts
void EUSCIA0_IRQHandler(){
    if (EUSCI_A0->IFG & BIT0){
        addItemCircBuf(RXBuf, EUSCI_A0->RXBUF);
    }
    if (EUSCI_A0->IFG & BIT1){
        if(isEmpty(TXBuf)) {
            EUSCI_A0->IFG &= ~BIT1;
            return;
        }
        EUSCI_A0->TXBUF = removeItem(TXBuf);
    }
}

// I2C interrupts
void EUSCIB0_IRQHandler() {
    // NACK received
    if(EUSCI_B0->IFG & EUSCI_B_IFG_NACKIFG) {
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_NACKIFG); // ACK not NACK
        EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTT; // send start message
    }

    // transmitting
    if(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG0) {
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_TXIFG0);
        if(isEmpty(TXBuf_i2c)){                    // if circbuf is empty
            EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_TXSTP; // transmit stop message
            EUSCI_B0->IFG &= ~(EUSCI_B_IFG_TXIFG0); // clear flag
        }
        else { // if not empty, send a byte
            EUSCI_B0->TXBUF = removeItem(TXBuf_i2c); // load what's in buffer
        }
    }

    // receiving
    if(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG0) {
        EUSCI_B0->IFG &= ~(EUSCI_B_IFG_RXIFG0); // clear flag
        addItemCircBuf(RXBuf_i2c, EUSCI_B0->RXBUF);    // throw in circbuf
    }
}
