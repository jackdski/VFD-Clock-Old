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

/* --- G L O B A L   V A R I A B L E S --- */

extern SwitchMode switch_select;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;

extern uint8_t doButtons;
extern uint8_t buttonCount;

extern CircBuf_t * RXBuf;
extern CircBuf_t * TXBuf;

extern uint8_t parse_request;


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
 * [ START_MSG | START_MSG | TEMP_MODE | END_MSG ]
 *
 *             Clock Mode Request Message
 * [ START_MSG | START_MSG | CLOCK_MODE | END_MSG ]
 */

/* --- P E R I P H E R A L S   F U N C T I O N S --- */

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
/*
 *   P5.0 -> Change Time Switch
 *   P5.1 -> '+' Button
 *   P5.2 -> '-' Button
 */
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


/* parse the message received */
uint8_t parse_rx_message(CircBuf_t * rxbuf) {
    UCA0IE &= ~(BIT0 | BIT1);  //Turn on interrupts for RX and TX

    uint8_t start[2];
    uint8_t msg_function;

    start[0] = removeItem(rxbuf);
    start[1] = removeItem(rxbuf);

//    // shift through buffer until START_MSG is found
//    while(start[0] != START_MSG && !isEmpty(rxbuf)) {
//        start[1] = start[0];
//        start[0] = removeItem(rxbuf);
//    }

    // check for double START_MSG
    if(start[1] == START_MSG) {
        msg_function = removeItem(rxbuf);

        if(msg_function == SET_TIME) {
            // set to setup mode
            P2->OUT = BIT2;
            hours = removeItem(rxbuf);
            minutes = removeItem(rxbuf);
            update_time(hours, minutes, seconds);// do update
            return 1;
        }

        else if(msg_function == TEMP_MODE) {
            switch_select = Temperature;    // set to temp mode
            P2->OUT = BIT0;
            return 2;
        }

        else if(msg_function == CLOCK_MODE) {
            switch_select = Clock;
            P2->OUT = BIT1;
            return 3;
        }
    }
    UCA0IE |= (BIT0 | BIT1);  //Turn on interrupts for RX and TX
    resetCircBuf(rxbuf);
    return 0;
}


/*   I N T E R R U P T   F U N C T I O N S   */

// switch mode control
void PORT5_IRQHandler() {
    // "+" Button
    if(P5->IFG & BIT1 && P5->IN & BIT0) {
        // if rising edge
        if(P5->IES & BIT1) {
            if(switch_select == Setup){
                P5->IES &= ~BIT1; // set to falling edge
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

    // "-" Button
    if(P5->IFG & BIT2 && P5->IN & BIT0) {
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
    P5->IFG = 0; // clear interrupt flags
}

// UART interrupts
void EUSCIA0_IRQHandler(){
    if (EUSCI_A0->IFG & BIT0){  // rx
        uint8_t val = EUSCI_A0->RXBUF;  // read received byte

        // if received signal is the end message and its not the only thing in the buffer,
        //      call parse_rx_message() to do the mode change
        if((val == END_MSG) && (get_length_buf(RXBuf) > 0))
            parse_rx_message(RXBuf);
        else
            EUSCI_A0->TXBUF = (val); // echo for now
            addItemCircBuf(RXBuf, val);
    }
    if (EUSCI_A0->IFG & BIT1){  // tx
        if(isEmpty(TXBuf)) {
            EUSCI_A0->IFG &= ~BIT1;
            return;
        }
        EUSCI_A0->TXBUF = removeItem(TXBuf);
    }
    EUSCI_A0->IFG = 0;
}
