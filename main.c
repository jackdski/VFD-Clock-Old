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

/*  Display initializes to all segments on
 *      -Defaults to 00:00:00
 *      -Tube order: 12:34:56
 */
// start with all 7-segs off for init-setup
volatile uint8_t hours = 0;//0b1111111;
volatile uint8_t minutes = 0;//0b1111111;
volatile uint8_t seconds = 0;//0b1111111;

volatile uint8_t onOff = 1;         // 0 = blanks, 1 = current time
volatile uint8_t setupChanged = 0;  // for setup mode
volatile uint8_t timeChanged;   // 1 if RTC changes value

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // configure pins
//	configure_all_pins();
//	configure_rtc();
	configure_uart();
	configure_buttons();
	configure_shift_pins();
	configure_setup_timer();

	__enable_irq();

	// Config Port 2 LED for debugging help
	P2->SEL0 &= ~(BIT0 | BIT1 | BIT2);
    P2->SEL1 &= ~(BIT0 | BIT1 | BIT2);
	P2->DIR |= (BIT0 | BIT1 | BIT2);
	P2->OUT &= ~(BIT0 | BIT1 | BIT2);


	/* SETUP START */

	/* SETUP LOOP - flashes until switch is moved to the SETUP position */
	/* MAY NOT BE NEEDED IF RTC SAVES OLD TIME */
//	while(!(P5->IN & BIT0)) { // actual loop
//	while(1) { // for testing just this loop
//	    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE;  // turn on timer interrupts
//        if(setupChanged) {
//            showTimeSetUp(onOff);
//            onOff ^= 1;
//            setupChanged = 0;
//        }
//    }
    updateTime(hours, minutes, seconds);
    configure_rtc();    // now config RTC

	/* SETUP END */

	/* MAIN LOOP */
	while(1) {
//	     NORMAL position - switch is LOW so run normal clock loop
	    if( !(P5->IN & BIT0) ) {
//            seconds = RTCSEC;
//            // make sure hrs, mins, and secs increments correctly
//            if(seconds == 0) {
//                if(minutes == 60) {
//                    minutes = 0;
//                    if(hours == 13) {
//                        hours = 0;
//                    }
//                    else {
//                        hours++;
//                    }
//                }
//                else {
//                    minutes++;
//                    P1->OUT ^= BIT0;
//                }
//            }
//            // blink light at 1Hz
//            if(seconds % 2 == 0) {
//                P1->OUT |= BIT0;
//            }
//            if(seconds % 2 != 0) {
//                P1->OUT &= ~BIT0;
//            }
//
//            // load newly calculated time to the displays
	        if( timeChanged ) {
	            updateTime(hours, minutes, seconds);
	            timeChanged = 0;
	        }
	        // else: don't change anything
	    }

	    // SETUP position - switch is HIGH so run time-picking mode
	    if( P5->IN & BIT0 ) {
	        uint32_t i;
	        for(i = 0; i < 25000; i++);
	        P1->OUT ^= BIT0;
	        // alternate between displaying the current time and blanks
	        if(setupChanged)
	            showTimeSetUp(onOff);
	            setupChanged = 0;
	    }
	}
}
