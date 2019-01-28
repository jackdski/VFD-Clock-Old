/**
 *  VFD Tube Clock
 *  Author: Jack Danielski
 *
 *  main.c
 */

#define     MAIN /* runs main loop */
#define     RUN_SETUP /* runs setup on startup */
//#define     SHIFT_REG_TEST  // displays 0-9 continuously
//#define     UART_TEST       // if a msg is properly receive light an LED
//#define     BUTTONS_TEST    // '+' = blue light toggle, '-' = green light toggle

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

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// create circular buffer for Bluetooth comm
    RXBuf = createCircBuf(8);
    TXBuf = createCircBuf(16);

    // configuration function calls
	configure_all_pins();
	configure_SystemClock();
	configure_rtc();
	configure_uart();
	configure_buttons();
	configure_leds();
	configure_shift_pins();

	__enable_irq();


    updateTime(hours, minutes, seconds);
    configure_rtc();    // now config RTC

	/* MAIN LOOP */
	while(1) {
	    // BLUETOOTH SETUP mode
	    if( P1->IN & BIT4 ) {
	        if( isFullCircBuf(RXBuf) ) {
                uint8_t str[8];
                int i = 0;
                for(i=0; i < 8; i++) {
                    str[i] = removeItem(RXBuf) - 48;
                }

                if(str[0] == '<' && str[7] == '>' ){
                    // take RXBuf items and convert from ASCII to decimal
                    hours = str[1] + str[2];
                    minutes = str[3] + str[4];
                    seconds = str[5] + str[6];
                    updateTime(hours, minutes, seconds);
                }
                else {  // print error message to try again
                    uint8_t * string = "ERROR...\n";
                    loadToBuf(TXBuf, string, 9);
                    if(!(isEmpty((CircBuf_t *)TXBuf))) {
                        EUSCI_A0->IFG |= BIT1;
                    }

                    string = "Try again...\n";
                    loadToBuf(TXBuf, string, 13);
                    if(!(isEmpty((CircBuf_t *)TXBuf))) {
                        EUSCI_A0->IFG |= BIT1;
                    }
                }
                if( (hours == 1) && (minutes == 2) && (seconds == 3) ) {
                    // light up green light
                    P2->OUT &= ~(BIT0 | BIT2);
                    P2->OUT |= BIT1;
                }
                else {
                    // light up red light
                    P2->OUT &= ~(BIT1 | BIT2);
                    P2->OUT |= BIT0;
                }
                resetCircBuf((CircBuf_t *)RXBuf);
	        }
	    }

	    // MANUAL SETUP position - switch is HIGH so run time-picking mode
	    if( P5->IN & BIT0 ) {
	        // turn on setup indication LED
	        P1->OUT |= BIT0;

	        // if not disabled already, disable RTC interrupts
            if(NVIC->ISER[(((uint32_t)(int32_t)RTC_C_IRQn) >> 5UL)] !=
                    (uint32_t)(1UL << (((uint32_t)(int32_t)RTC_C_IRQn) & 0x1FUL))) {
                NVIC_DisableIRQ(RTC_C_IRQn);
            }

            // if + button is pressed
            if(doButtons &= 0b1) {
                if(buttonCount >= 10)
                    doButtons = 0b1001;
            }
            // if - button is pressed
            if(doButtons &= 0x10) {
                if(buttonCount >= 10)
                    doButtons = 0b10010000;
            }

            // update tubes if safe for reading (necessary to check?)
            if(RTCCTL13 & RTC_C_CTL13_RDY) { // if safe for reading
                updateTime(RTCHOUR, RTCMIN, RTCSEC);
            }
	        P1->OUT |= BIT0; // indicate SETUP
	    }

	    // NORMAL position - waits for interrupts
	    else { // !(P5->IN & BIT0)
	        // if not enabled already, enable RTC interrupts
	        if(NVIC->ISER[(((uint32_t)(int32_t)RTC_C_IRQn) >> 5UL)] ==
	                (uint32_t)(1UL << (((uint32_t)(int32_t)RTC_C_IRQn) & 0x1FUL))) {
	            NVIC_EnableIRQ(RTC_C_IRQn);
	            buttonCount = 0; // reset button counter too
	        }
	        P1->OUT &= ~BIT0; // indicate NORMAL
	    }
	}
}
