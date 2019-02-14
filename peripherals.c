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
#include "peripherals.h"


/* M E S S A G E   M A C R O S */

#define    START_MSG       0xFE
#define    SET_TIME        0x11
#define    TEMP_MODE       0x22
#define    CLOCK_MODE      0x33
#define    END_MSG         0x0F

/*                  Message Structure
 * [ START_MSG | START_MSG | FUNCTION | DATA ... DATA | SIZE | END_MSG ]
 *
 *                  Time Setting Message
 * [ START_MSG | START_MSG | SET_TIME | HOURS | MINUTES | END_MSG ]
 *
 *               Temp. Mode Request Message
 * [ START_MSG | START_MSG | TEMP_MODE | TEMP_MODE | END_MSG ]
 *
 *             Clock Mode Request Message
 * [ START_MSG | START_MSG | CLOCK_MODE | CLOCK_MODE | END_MSG ]
 */

extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;

extern uint8_t doButtons;
extern uint8_t buttonCount;

extern CircBuf_t * RXBuf;
extern CircBuf_t * TXBuf;

extern uint8_t parse_request;


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

/* set all unused pins to low for power reasons */
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

uint8_t parse_rx_message(CircBuf_t * rxbuf) {
    uint8_t start[2];
    start[0] = removeItem(rxbuf);
    start[1] = removeItem(rxbuf);

    if((start[0] == START_MSG) || (start[1] == START_MSG)) {
        if(get_length_buf(rxbuf) == 4) {  // set time message, 5 bytes long
            // set to SETUP mode
            P2->OUT = BIT2;
            hours = removeItem(rxbuf);
            minutes = removeItem(rxbuf);
            return 1;
        }
        else if(get_length_buf(rxbuf) == 3) {
            uint8_t new_mode[2];
            new_mode[0] = removeItem(rxbuf);
            new_mode[1] = removeItem(rxbuf);
            if((new_mode[0] == TEMP_MODE) || ((new_mode[1] == TEMP_MODE))) {
                // set to temp mode
                P2->OUT = BIT0;
                return 1;
            }
            else if((new_mode[0] == CLOCK_MODE) || ((new_mode[1] == CLOCK_MODE))) {
                // set to clock mode
                P2->OUT = BIT1;
                return 1;
            }
        }
        else {
            return 0;
        }
    }
    return 0;
}


// UART interrupts
void EUSCIA0_IRQHandler(){
    if (EUSCI_A0->IFG & BIT0){
        if(EUSCI_A0->RXBUF == END_MSG)
            parse_request = 1;
        EUSCI_A0->TXBUF = EUSCI_A0->RXBUF; // echo for now
        addItemCircBuf(RXBuf, EUSCI_A0->RXBUF);
    }
    if (EUSCI_A0->IFG & BIT1){
        //Transmit Everything in TXBuf
        if(isEmpty(TXBuf)) {
            EUSCI_A0->IFG &= ~BIT1;
            return;
        }
        EUSCI_A0->TXBUF = removeItem(TXBuf);
    }
}
