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

extern SwitchMode switch_select;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;

extern uint8_t doButtons;
extern uint8_t buttonCount;

extern CircBuf_t * RXBuf;
extern CircBuf_t * TXBuf;

void configure_SystemClock(){
/*
 * configure the system clock to have DCO be 8MHz
 */
    CS-> KEY = CS_KEY_VAL; //Unlock module for register access
    CS-> CTL0 = 0;     //Reset tuning parameters
    CS-> CTL0 = (BIT(23) | CS_CTL0_DCORSEL_3);     //Setup DCO Clock

    //Select ACLO = REFO, SMCLK MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;       //Lock CS module for register access.
}


void configure_uart(){
/*
 * configure UART on P1.2 (RX) and P1.3 (TX) to be used for Bluetooth communication
 */
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
/*   P5.0 -> Change Time Switch
 *   P5.1 -> '+' Button
 *   P5.2 -> '-' Button
 *   P5.4 -> Temp/Time Select   */

    // config switch and button SEL reg's
    P5->SEL0 &= ~(BIT0 | BIT1 | BIT2);
    P5->SEL1 &= ~(BIT0 | BIT1 | BIT2);

    // config switch and +/- buttons direction and pulldown resistor
    P5->DIR &= ~(BIT0 | BIT1 | BIT2 | BIT4);    // put to input direction
    P5->REN |= (BIT0 | BIT1 | BIT2 | BIT4);     // enable pullup/down resistor
    P5->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT4);    // select pulldown resistor
    P5->IE |= (BIT0 | BIT1 | BIT2 | BIT4);      // enable interrupts on buttons and switches

    NVIC_EnableIRQ(PORT5_IRQn);
}

void configure_leds() {
    // config LED
    P1->SEL0 &= ~(BIT0);
    P1->SEL1 &= ~(BIT0);
    P1->DIR |=  (BIT0);
    P1->OUT &= ~(BIT0);

    // Config Port 2 LED for debugging help
    P2->SEL0 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4);
    P2->SEL1 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4);
    P2->DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4);
    P2->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4);

    P5->SEL0 &= ~(BIT3);
    P5->SEL1 &= ~(BIT3);
    P5->DIR |= BIT3;
    P5->OUT &= ~(BIT3);
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

void PORT5_IRQHandler() {
    // Setup Switch
    if(P5->IFG & BIT0) {    // change mode to Setup
        // maybe use (P5->IN & BIT0) instead
        if((P5->IES & BIT0) && (P5->IN & BIT4)) {    // if Setup turns on and Temp Mode is on
            P5->IES &= ~(BIT0); // set to trigger flag on falling edge
            switch_select = Temperature; // or setup??
        }
        else if((P5->IES & BIT0) && !(P5->IN & BIT4)) { // if Setup turning on and Temp Mode off
            P5->IES |= BIT0;    // trigger flag on rising edge
            switch_select = Setup;
        }
        else if(!(P5->IES & BIT0) && (P5->IN & BIT4)) {  // if Setup turning off and Temp mode on
            P5->IES |= BIT0;    // trigger flag on rising edge
            switch_select = Temperature;
        }
        else if(!(P5->IES & BIT0) && !(P5->IN & BIT4)) { // if Setup turning off and Temp mode off
            P5->IES |= BIT0;    // trigger flag on rising edge
            switch_select = Normal;
        }
    }

    // "+" Button
    if(P5->IFG & BIT1) {   // if '+' button Change Time switch is on
        if((P5->IN & BIT4) && (P5->IN & BIT0)) {    // make sure in time and setup modes
            // if rising edge
            if(P5->IES & BIT1) {
                if(switch_select == Setup){
                    P5->IES &= ~(BIT1); // set to falling edge
                    doButtons = 0b01;
                    RTCSEC = 0;
                    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;  // enable timer
                }
            }

            // if falling edge
            else if(P5->IES & ~BIT1) {
                if(switch_select == Setup){
                    P5->IES &= BIT1; // set to rising edge
                    doButtons = 0b10;
                    RTCSEC = 0;
                    buttonCount = 0; // reset button count
                    TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIE);  // disable timer
                    TIMER_A0->R = 0;                             // clear timer count
                }
            }
        }
    }

    // "-" Button
    if(P5->IFG & BIT2) {
        if((P5->IN & BIT4) && (P5->IN & BIT0)) {    // make sure in time mode
            // if rising edge
            if(P5->IES & BIT2) {
                if(switch_select == Setup){
                    P5->IES &= ~BIT2; // set to falling edge
                    doButtons = 0b00010000;
                    RTCSEC = 0;
                    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;  // enable timer
                }
            }

            // if falling edge
            else if(P5->IES & ~BIT2) {
                if(switch_select == Setup){
                    P5->IES &= BIT2; // set to rising edge
                    doButtons = 0b10010000;
                    RTCSEC = 0;
                    buttonCount = 0; // reset button count
                    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;  // disable timer
                    TIMER_A0->R = 0;                          // clear timer count
                }
            }
        }
    }

    // Temp/Time Switch
    if(P5->IFG & BIT4) {
        // maybe use (P5->IN & BIT0) instead
        if((P5->IES & BIT4) && (P5->IN & BIT0)) {    // if go to temp and in setup mode
            P5->IES &= ~(BIT4); // set to trigger flag on falling edge
            switch_select = Temperature; // or setup??
        }
        else if((P5->IES & BIT4) && !(P5->IN & BIT0)) { // if go to temp and in show time mode
            P5->IES |= BIT4;    // trigger flag on rising edge
            switch_select = Temperature;
        }
        else if(!(P5->IES & BIT4) && (P5->IN & BIT0)) {  // if go to time and in setup mode
            P5->IES |= BIT4;    // trigger flag on rising edge
            switch_select = Setup;
        }
        else if(!(P5->IES & BIT4) && !(P5->IN & BIT0)) { // if go to time and in show time mode
            P5->IES |= BIT4;    // trigger flag on rising edge
            switch_select = Normal;
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
        //Transmit Stuff
        if(isEmpty(TXBuf)) {
            EUSCI_A0->IFG &= ~BIT1;
            return;
        }
        EUSCI_A0->TXBUF = removeItem(TXBuf);
    }
}
