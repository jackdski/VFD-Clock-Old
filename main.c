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
#include "i2c.h"

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
CircBuf_t * RXBuf_i2c;
CircBuf_t * TXBuf_i2c;

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
    RXBuf_i2c = createCircBuf(8);
    TXBuf_i2c = createCircBuf(8);

    // configuration function calls
//	configure_all_pins();
	configure_SystemClock();
//	configure_rtc();
//	configure_uart();
	configure_i2c();
//	configure_buttons();
	configure_leds();
//	configure_shift_pins();

	__enable_irq();
    P1->OUT &= ~BIT0;
    uint8_t temp;

    set_mode_active();      // set temp sensor active
    set_oversample_rate(7); // set oversample rate
    enableEventFlags();     // enable temp sensor event flags


	/* MAIN LOOP */
	while(1) {
//        readRegister(0x00);   // status
//        readRegister(0x00);   // status
	    temp = read_temp_c();   // get temp
	    int i;
	    for(i=0;i < 5000000; i++);

	}
}
