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

//void configure_uart(){
//    //Configure UART pins, set 2-UART pins to UART mode
//    P1->SEL0 |=  (BIT2 | BIT3);
//    P1->SEL1 &= ~(BIT2 | BIT3);
//
//    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;     //Put eUSCI in reset
//    EUSCI_A0->CTLW0 |= (BIT7);                  //Select Frame parameters and source
//    EUSCI_A0->BRW = 78;                         //Set Baud Rate
//    EUSCI_A0->MCTLW |= (BIT0 | BIT5);           //Set modulator bits
//    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);  //Initialize eUSCI
//
//    EUSCI_A0->IFG &= ~(BIT1 | BIT0);
//    UCA0IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
//    NVIC_EnableIRQ(EUSCIA0_IRQn);
//}

void configure_buttons() {
    // P1.1 is + Button
    // P1.5 is enable switch for 'Set Time'
    // P1.4 is - Button
    P1->SEL0 &= ~(BIT0 | BIT1 | BIT5 | BIT4);
    P1->SEL1 &= ~(BIT0 | BIT1 | BIT5 | BIT4);

    // LED Stuff
    P1->DIR |=  (BIT0);
    P1->OUT &= ~(BIT0);

    // Switch and +/- button stuff
    P1->DIR &= ~(BIT1 | BIT4 | BIT5);  // put to input direction
    P1->REN |= (BIT1 | BIT4 | BIT5);    // enable pullup/down resistor
    P1->OUT &= ~(BIT1 | BIT4 | BIT5);   // enable pulldown resistor
}

void PORT1_IRQHandler() {
    if(P1->IFG & BIT1) {
        seconds = 0;
        minutes += 1;
        update_tubes(hours, minutes, seconds);
    }
    else {
        return;
    }

    if(P1->IFG & BIT4) {
        seconds = 0;
        minutes -= 1;
        update_tubes(hours, minutes, seconds);
    }
    else {
        return;
    }
}
