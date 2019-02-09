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

//extern Mode state;
extern uint8_t hours;
extern uint8_t minutes;
extern uint8_t seconds;

//extern uint8_t onOff; // 0 = blanks, 1 = current time
//extern uint8_t setupChanged;

extern uint8_t doButtons;
extern uint8_t buttonCount;
extern uint8_t update_request;

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
void configure_setup_timer(){
    TIMER_A0->R = 0;                    // Clear timer count
    TIMER_A0->CTL = SET_CTL;            // Set to SMCLK, Up mode (BIT9 ON)
    TIMER_A0->CCR[0] = COUNT_TO;        // Count to 50ms
    TIMER_A0->CCTL[0] |= SET_CCTL;      // TACCR0 interrupt enabled
    NVIC_EnableIRQ(TA0_0_IRQn);
}

void TA0_0_IRQHandler() {

//    TIMER_A0->CTL &= ~(BIT1);  //Turn off timer interrupts
//    NVIC_DisableIRQ(TA0_0_IRQn);
    TIMER_A0->CCTL[0] &= !TIMER_A_CCTLN_CCIE; // disable interrupt
    buttonCount++; // increment

    if(doButtons == 0b00001001) // 0x09 == increment hours
        hours++;
    else if(doButtons == 0b00000001) // 0x01 == increment mins
        minutes++;
    else if(doButtons == 0b10010000) // 0x90 == decrement hours
        hours--;
    else if(doButtons == 0b00010000)  // 0x10 == decrement minutes
        minutes--;
    update_request = 1;
//    P2->OUT ^= BIT2;

    //Clear the timer interupt
    TIMER_A0->CCTL[0] &= ~(BIT0);
}
