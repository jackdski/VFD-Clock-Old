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
    /* Enables the MSP432 in LPM0_LF_VCORE0 sleep power mode */
    P1->OUT ^= BIT0; // for debugging

    // Turn off PSS high-side supervisors
    PSS->KEY = PSS_KEY_KEY_VAL;
    PSS->CTL0 |= PSS_CTL0_SVSMHOFF;
    PSS->KEY = 0;

    // Enable all SRAM bank retentions prior to going to LPM3
    SYSCTL->SRAM_BANKRET |= SYSCTL_SRAM_BANKRET_BNK7_RET;

}

void enable_low_power_mode() {
//    enters key value, requests LPM0_LDO_VCORE0 change
//    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
//    PCM->CTL0 |= (PCM_CTL0_KEY_VAL | PCM_CTL0_CPM__LPM0_LDO_VCORE0);
//    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
//    PCM->CTL0 = 0; // lock register
//    if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG) // error
//        P2->OUT = BIT0; // red light
//    else{
//        P2->OUT = BIT1; // green light
//    }
//    SCB->SCR &= ~(SCB_SCR_SEVONPEND_Pos); // SEVONPEND, only enabled interrupts can wake
//    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);
//    SCB->SCR |= (SCB_SCR_SLEEPONEXIT_Msk);
//    // Ensures the SLEEPONEXIT mask is set.
//    __DSB();
//    __WFI();
//    Go to LPM0
//    __sleep();

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    PCM->CTL0 &= ~(PCM_CTL0_LPMR__LPM3);
    if(PCM->IFG & PCM_CLRIFG_CLR_LPM_INVALID_CLK_IFG) {
        P2->OUT = BIT0;
    }
    else if(PCM->IFG & PCM_CLRIFG_CLR_LPM_INVALID_TR_IFG ){
        P2->OUT = BIT0;
    }
    else {
        P2->OUT = BIT1;
    }
    __DSB();
    __WFE();
}
