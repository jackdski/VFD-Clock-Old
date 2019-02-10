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

// Switch Mode Select
SwitchMode switch_select = Setup;

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


// 0b0000_0001=do + button, 0b0001_0000=do - button
// 0b1000 = increment hours, 0b0000 = increment minutes
volatile uint8_t doButtons = 0b00000000;
volatile uint8_t buttonCount = 0;
volatile uint8_t update_request = 0;   // 1 if update is need from button press
volatile uint8_t temperature_update_request = 0;
volatile uint16_t temperature_timer_count = 0;


void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // configuration function calls
	configure_all_pins();
	configure_SystemClock();
//	configure_rtc();
//	configure_uart();
	configure_buttons();
	configure_leds();
	configure_shift_pins();
	configure_setup_timer();
	configure_temperature_timer();

	__enable_irq();


    updateTime(hours, minutes, seconds);

	/* MAIN LOOP */
	while(1) {
	    if(switch_select == Normal) {
            if(NVIC->ISER[(((uint32_t)(int32_t)RTC_C_IRQn) >> 5UL)] ==
                    (uint32_t)(1UL << (((uint32_t)(int32_t)RTC_C_IRQn) & 0x1FUL))) {
                NVIC_EnableIRQ(RTC_C_IRQn);
            }
	    }
	    else if(switch_select == Setup) {
//	        P2->OUT |= (BIT0 | BIT2); // indicate setup mode
	        if(update_request == 1) {
//	            updateTime(hours, minutes, seconds);// do update
	            update_request = 0;                 // reset update request
                P2->OUT ^= BIT2;
	        }
	    }
	    else if(switch_select == Temperature) {
	        // need to add way of reseting timer_count
	        if(temperature_update_request == 1) {
//	             updateTemp(); // updates the display to show temp
	            P2->OUT ^= BIT2;
	            temperature_update_request = 0;
	        }
	    }
	}
}

