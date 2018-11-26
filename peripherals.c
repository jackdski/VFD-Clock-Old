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

//extern Mode state;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;

extern uint8_t doButtons;
extern uint8_t buttonCount;

// configure on P3.2 and P3.3 to be used for UART, if necessary
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

    // config LED
    P1->SEL0 &= ~(BIT0);
    P1->SEL1 &= ~(BIT0);
    P1->DIR |=  (BIT0);
    P1->OUT &= ~(BIT0);

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

/* set all unused pins to low for power reasons */
void configure_all_pins() {
    P1->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P1->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P2->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P2->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P3->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P4->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P4->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P5->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P5->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P6->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P6->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P7->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P7->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P8->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P8->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P9->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P9->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

    P10->DIR &=  ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
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
