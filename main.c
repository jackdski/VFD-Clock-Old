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

/*
 * Time initialization declarations
 *  -Defaults to 00:00:00
 *  -Tube order: 12:34:56
 */
volatile uint8_t hours = 0;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // configure peripherals
	configure_rtc();
	configure_uart();
	configure_buttons();

	// startup config
	uint8_t run_startup = 1;
	uint8_t count_presses = 0;

	/* unused while displaying to clock is being written */
	/* Flash until switch is moved to the SETUP position */
	/* while(!(P5->IN & BIT0)) {} */
	while(1) {
	    // NORMAL position - switch is low so run normal clock loop
	    if( !(P5->IN & BIT0) ) {
            seconds = RTCSEC;
            if(seconds == 0) {
                if(minutes == 59) {
                    minutes = 0;
                    if(hours == 12) {
                        hours = 0;
                    }
                    else {
                        hours++;
                    }
                }
                else {
                    minutes++;
                    P1->OUT ^= BIT0;
                }
            }
            if(seconds % 2 == 0) {
                P1->OUT |= BIT0;
            }
            if(seconds % 2 != 0) {
                P1->OUT &= ~BIT0;
            }
            updateTime(hours, minutes, seconds);
	    }

	    // SETUP position - switch is high so run time-picking mode
	    if( P5->IN & BIT0 ) {
	        /* to do:
	         *  - write function to flash current input
	         *  - write function to flash a blank display
	         *  - set up 0.5s timer to be used in these functions
	         */
	        uint32_t i;
	        for(i = 0; i < 25000; i++);
	        P1->OUT ^= BIT0;
	    }
	}
}
