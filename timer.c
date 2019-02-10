/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * timer.c
 * timer for flashing pattern on startup and changing time
 */

#include "msp.h"
#include "timer.h"
#include <stdint.h>
#include "tubes.h"

/*  G L O B A L   V A R I A B L E S  */
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;

extern uint8_t doButtons;
extern uint8_t buttonCount;
extern uint8_t update_request;
extern uint8_t temperature_update_request;
extern uint16_t temperature_timer_count;

///* timer used for button presses */
//void enableSystick(uint16_t microseconds) {
//    SysTick->LOAD = 3 * microseconds;;
//    SysTick->CTRL = BIT0 | BIT1 | BIT2; // enable bits
//}
//
///* disable the timer */
//void disableSystick() {
//    SysTick->CTRL &= ~(BIT0 | BIT1 | BIT2);
//}
//
///* disable the timer after it counts down all the way */
//void SysTick_Handler() {
//    buttonCount++; // increment
//    if(doButtons == 0b00001001) // 0x09
//        RTCHOUR++;
//    else if(doButtons == 0b00000001) // 0x01
//        RTCMIN++;
//    else if(doButtons == 0b10010000) // 0x90
//        RTCHOUR--;
//    else if(doButtons == 0b00010000)
//        RTCMIN--;
//}


/* set up 50ms timer */
void configure_setup_timer() {
    TIMER_A0->R = 0;                    // Clear timer count
    TIMER_A0->CTL = SET_CTL;            // Set to SMCLK, Up mode (BIT9 ON)
    TIMER_A0->CCR[0] = COUNT_TO_25;        // Count to 25ms
    TIMER_A0->CCTL[0] |= SET_CCTL;      // TACCR0 interrupt enabled
    NVIC_EnableIRQ(TA0_0_IRQn);
}

/* set up 500ms timer */
void configure_temperature_timer() {
    TIMER_A1->R = 0;
    TIMER_A1->CTL = SET_CTL;
    TIMER_A1->CCR[0] = COUNT_TO_25;
    TIMER_A1->CCTL[0] |= SET_CCTL;
    NVIC_EnableIRQ(TA1_0_IRQn);
}

void TA0_0_IRQHandler() {

//    TIMER_A0->CTL &= ~(BIT1);  //Turn off timer interrupts
//    NVIC_DisableIRQ(TA0_0_IRQn);
//    TIMER_A0->CCTL[0] &= !TIMER_A_CCTLN_CCIE; // disable interrupt
    if(buttonCount == 2) {
        P2->OUT ^= BIT4;
        if(doButtons == 0b00001001) // 0x09 == increment hours
            hours++;
        else if(doButtons == 0b00000001) // 0x01 == increment mins
            minutes++;
        else if(doButtons == 0b10010000) // 0x90 == decrement hours
            hours--;
        else if(doButtons == 0b00010000)  // 0x10 == decrement minutes
            minutes--;
        update_request = 1;
        buttonCount = 0;
     }
    else {
        buttonCount++; // increment
        P2->OUT ^= BIT3;
    }
    TIMER_A0->CCTL[0] &= ~(BIT0);  //Clear the timer interrupt
}

void TA1_0_IRQHandler() {
    if(temperature_timer_count == 400) {
        // will be get temperature sensor data
        // for now turn Blue LED on
        P2->OUT ^= BIT4;
        temperature_update_request = 1;
        temperature_timer_count = 0;
    }
    else{
        temperature_timer_count++;
        P2->OUT ^= BIT3;
    }
    TIMER_A1->CCTL[0] &= ~(BIT0); // clear timer interrupt
}
