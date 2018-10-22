/*
 *
 * Jack Danielski
 * VFD Tube Clock
 *  peripherals.h
 *
 */

#include "msp.h"
#include "rtc.h"
#include "tubes.h"

//extern Mode state;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;

// configure on P3.2 and P3.3 to be used for UART
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
    P5->DIR &= ~(BIT1 | BIT4 | BIT5);  // put to input direction
    P5->REN |= (BIT1 | BIT4 | BIT5);    // enable pullup/down resistor
    P5->OUT &= ~(BIT1 | BIT4 | BIT5);   // enable pulldown resistor
}

// +/- control
void PORT5_IRQHandler() {
    // "+" Button
    if(P5->IFG & BIT1) {
        seconds = 0;
        minutes += 1;
//        update_tubes(hours, minutes, seconds);
        updateTime(hours, minutes, seconds);
    }
    else {
        return;
    }

    // "-" Button
    if(P5->IFG & BIT2) {
        seconds = 0;
        minutes -= 1;
//        update_tubes(hours, minutes, seconds);
        updateTime(hours, minutes, seconds);
    }
    else {
        return;
    }
}
