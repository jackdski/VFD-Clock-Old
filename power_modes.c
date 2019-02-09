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


void configure_low_power_modes(){
    /*
     * Puts the MSP432 in LPM0_LF_VCORE0 sleep power mode
     */
    P1->OUT ^= BIT0; // for debugging

    // Turn off PSS high-side supervisors
    PSS->KEY = PSS_KEY_KEY_VAL;
    PSS->CTL0 |= PSS_CTL0_SVSMHOFF;
    PSS->KEY = 0;

    // Enable all SRAM bank retentions prior to going to LPM3
    SYSCTL->SRAM_BANKRET |= SYSCTL_SRAM_BANKRET_BNK7_RET;

}

void enable_low_power_mode() {
    // enters key value, requests LPM0_LDO_VCORE0 change
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    PCM->CTL0 |= (PCM_CTL0_KEY_VAL | PCM_CTL0_CPM__LPM0_LDO_VCORE0);
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    PCM->CTL0 = 0; // lock register
    if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG) // error
        P2->OUT |= BIT1;
    SCB->SCR &= ~(SCB_SCR_SEVONPEND_Msk); // SEVONPEND, only enabled interrupts can wake
    SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Pos);
    // Ensures the SLEEPONEXIT mask is set.
    __DSB();

    // Go to LPM0
    __sleep();
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
