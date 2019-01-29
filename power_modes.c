/*
 * VFD Tube Clock
 * Jack Danielski
 *
 *  power_modes.c
 */

#include "msp.h"
#include "rtc.h"
#include <stdint.h>
#include "timer.h"
#include "power_modes.h"


void enable_sleep_mode(){
    /*
     * Puts the MSP432 in LPM0_LF_VCORE0 sleep power mode
     */
    P1->OUT ^= BIT0; // for debugging
    SCB->SCR |= (BIT4); // SEVONPEND
    SCB->SCR &= ~(BIT1 | BIT2); // SLEEPDEEP and SLEEPONEXIT

    // enters key value, requests LPM0_LDO_VCORE0 change
    PCM->CTL0 |= (PCM_CTL0_KEY_VAL | PCM_CTL0_CPM__LPM0_LDO_VCORE0);

    PCM->CTL0 = 0; // lock register
}

void wake_up_mode(){
    /*
     *  Puts the MSP432 in AM_LDO_VCORE0 active power mode
     */
    P1->OUT ^= BIT0; // for debugging
    // enters key value, sets and requests AM_LDO_VCORE0
    PCM->CTL0 |= (PCM_CTL0_KEY_VAL | PCM_CTL0_CPM__AM_LDO_VCORE0 | PCM_CTL0_AMR_0);
    PCM->CTL0 = 0; // lock register
}
