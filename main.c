/**
 *  VFD Tube Clock
 *  Author: Jack Danielski
 *
 *  main.c
 */


#include "msp.h"

#include "circbuf.h"
#include "peripherals.h"
#include "power_modes.h"
#include "rtc.h"
#include "timer.h"
#include "tubes.h"
#include "i2c.h"


/* --- G L O B A L   V A R I A B L E S --- */

// Switch Mode Select
SwitchMode switch_select = Clock;

/*  Display initializes to all segments on
 *      -Defaults to 00:00:00
 *      -Tube order: 12:34:56       */
volatile uint8_t hours = 0;//0b1111111;
volatile uint8_t minutes = 0;//0b1111111;
volatile uint8_t seconds = 0;//0b1111111;

volatile uint8_t temperature;

CircBuf_t * TXBuf;
CircBuf_t * RXBuf;

//volatile uint8_t onOff = 1;         // 0 = blanks, 1 = current time
//volatile uint8_t setupChanged = 0;  // for setup mode
//volatile uint8_t timeChanged;   // 1 if RTC changes value


// 0b0000_0001=do + button, 0b0001_0000=do - button
// 0b1000 = increment hours, 0b0000 = increment minutes
volatile uint8_t doButtons = 0b00000000;
volatile uint8_t buttonCount = 0b00000000;
volatile uint8_t update_request;   // 1 if update is need from button press
volatile uint8_t temperature_update_request = 0; // sample temperature if 1
volatile uint16_t temperature_timer_count = 0;   // count to 400 for 5s


/* -----M A I N----- */


void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    RXBuf = createCircBuf(8);   // circular buffer for BLE RX
    TXBuf = createCircBuf(16);  // circular buffer for BLE TX

    // configuration function calls
	//configure_all_pins();
	configure_SystemClock();
	configure_uart();
	configure_buttons();
	configure_leds();
	configure_shift_pins();
	configure_temperature_timer();
	configure_i2c();

	__enable_irq();

    update_time(hours, minutes, seconds);   // display time
    configure_rtc();    // configure RTC last

	/*--MAIN LOOP--*/
	while(1) {
        if(switch_select == Clock) {
            // if RTC interrupts are off turn them on
            if(NVIC->ISER[(((uint32_t)(int32_t)RTC_C_IRQn) >> 5UL)] ==
                    (uint32_t)(1UL << (((uint32_t)(int32_t)RTC_C_IRQn) & 0x1FUL))) {
                NVIC_EnableIRQ(RTC_C_IRQn);
            }
            enable_low_power_mode();
        }
        else if(switch_select == Setup) {
            P2->OUT |= (BIT0 | BIT2); // indicate setup mode
            if(update_request == 1) {
                update_time(hours, minutes, seconds);// do update
                update_request = 0;                 // reset update request
            }
        }
        else if(switch_select == Temperature) {
            // need to add way of reseting timer_count
            set_mode_active();      // set temperature sensor active
            set_oversample_rate(7); // set oversample rate
            enable_event_flags();   // enable temperature sensor events

            if(temperature_update_request == 1) {
                temperature = read_temp_f();    // get temperature
                update_temperature(temperature);// put latest temperature on display
                temperature_update_request = 0;
            }
            set_mode_standby();
        }
	}
}
