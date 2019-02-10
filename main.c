/**
 *  VFD Tube Clock
 *  Author: Jack Danielski
 *
 *  main.c
 */

#include "msp.h"
#include "rtc.h"
#include "tubes.h"
#include "peripherals.h"
#include "timer.h"
#include "circbuf.h"
#include "power_modes.h"

/*  Display initializes to all segments on
 *      -Defaults to 00:00:00
 *      -Tube order: 12:34:56
 */
// start with all 7-segs off for init-setup
volatile uint8_t hours = 0;//0b1111111;
volatile uint8_t minutes = 0;//0b1111111;
volatile uint8_t seconds = 0;//0b1111111;

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // configuration function calls
	configure_all_pins();
	configure_SystemClock();
	configure_low_power_modes();
//	configure_rtc();
//	configure_uart();
	configure_buttons();
	configure_leds();
//	configure_shift_pins();

	__enable_irq();


//    updateTime(hours, minutes, seconds);
//    configure_rtc();    // now config RTC

	/* MAIN LOOP */
	while(1) {
	    if(CoreDebug->DHCSR & BIT(18)) {
	        P2->OUT |= BIT2;
	    }

	    if(NVIC->ISER[(((uint32_t)(int32_t)RTC_C_IRQn) >> 5UL)] ==
            (uint32_t)(1UL << (((uint32_t)(int32_t)RTC_C_IRQn) & 0x1FUL))) {
	        NVIC_EnableIRQ(RTC_C_IRQn);
	    }
	}
}
