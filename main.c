/**
 *  VFD Tube Clock
 *  Author: Jack Danielski
 *
 *  main.c
 */

//#define     PATTERN

#include "msp.h"
#include "rtc.h"
#include "tubes.h"
#include "peripherals.h"
#include "timer.h"
#include "circbuf.h"

/*  Display initializes to all segments on
 *      -Defaults to 00:00:00
 *      -Tube order: 12:34:56
 */
// start with all 7-segs off for init-setup
volatile uint8_t hours = 0;//0b1111111;
volatile uint8_t minutes = 0;//0b1111111;
volatile uint8_t seconds = 0;//0b1111111;

CircBuf_t * TXBuf;
CircBuf_t * RXBuf;

//volatile uint8_t onOff = 1;         // 0 = blanks, 1 = current time
//volatile uint8_t setupChanged = 0;  // for setup mode
//volatile uint8_t timeChanged;   // 1 if RTC changes value


// 0b0000_0001=do + button, 0b0001_0000=do - button
// 0b1000 = increment hours, 0b0000 = increment minutes
volatile uint8_t doButtons = 0;
volatile uint8_t buttonCount = 0;

uint8_t pattern_A = 0x00; // all off
uint8_t pattern_B = 0xAA; // 1/2 on, 1/2 off
uint8_t pattern_C = 0b11111111; // all on
uint8_t pattern_select = 0;

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // configuration function calls
	configure_all_pins();
	configure_SystemClock();
	configure_leds();
	configure_shift_pins();

	__enable_irq();

	update_time(hours, minutes, seconds);
//    configure_rtc();    // now config RTC
    uint32_t i;
	/* MAIN LOOP */
	while(1) {
//        // if not enabled already, enable RTC interrupts
//        if(NVIC->ISER[(((uint32_t)(int32_t)RTC_C_IRQn) >> 5UL)] ==
//                (uint32_t)(1UL << (((uint32_t)(int32_t)RTC_C_IRQn) & 0x1FUL))) {
//            NVIC_EnableIRQ(RTC_C_IRQn);
//            buttonCount = 0; // reset button counter too
//        }

#ifndef PATTERN
	    disable_output(1);
        // shiftOut(pattern_B)
        P4->OUT &= ~(BIT1); // latch (!SRCLR), set P4.1 low
        // write the values to the tubes
        shift_out(1, pattern_B);
        P4->OUT |= BIT1;    // set latch (!SRCLR0) high again
        enable_output(1);
#endif

#ifdef  PATTERN
        if(pattern_select == 0) {
	        // shiftOut(pattern_A)
            disable_output(1);
	        P4->OUT &= ~(BIT1); // latch (!SRCLR), set P4.1 low
	        // write the values to the tubes
	        shiftOut(1, pattern_A);
	        P4->OUT |= BIT1;    // set latch (!SRCLR0) high again
	        enable_output(1);
	    }
	    else if(pattern_select == 1) {
	        // shiftOut(pattern_B)
	        disable_output(1);
	        P4->OUT &= ~(BIT1); // latch (!SRCLR), set P4.1 low
	        // write the values to the tubes
	        shiftOut(1, pattern_B);
	        P4->OUT |= BIT1;    // set latch (!SRCLR0) high again
	        enable_output(1);
	    }
        else if(pattern_select == 2) {
            // shiftOut(pattern_B)
            disable_output(1);
            P4->OUT &= ~(BIT1); // latch (!SRCLR), set P4.1 low
            // write the values to the tubes
            shiftOut(1, pattern_C);
            P4->OUT |= BIT1;    // set latch (!SRCLR0) high again
            enable_output(1);
        }

	    if(pattern_select == 2) {
	        pattern_select = 0;
	    }
	    else {
	        pattern_select++;
	    }
#endif

	    for(i = 0; i < 500000; i++);
        P2->OUT ^= BIT1; // indicate MSP432 is on
	}
}
