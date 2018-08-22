/**
 *
 *  VFD Tube Clock
 *  main.c
 *
 */

#include "msp.h"
#include "rtc.h"
#include "tubes.h"
#include "peripherals.h"

/*
 * Time initialization declarations
 *  -Defaults to 00:00:00
 *  -Tube order: 12:34:56
 */
volatile uint8_t hours = 0;
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;

/*
 * Segment declarations
 */
// Tube 1
vfdSeg_t * oneA;
vfdSeg_t * oneB;
vfdSeg_t * oneC;
vfdSeg_t * oneD;
vfdSeg_t * oneE;
vfdSeg_t * oneF;
vfdSeg_t * oneG;
vfdSeg_t * oneDP;
// Tube 2
vfdSeg_t * twoA;
vfdSeg_t * twoB;
vfdSeg_t * twoC;
vfdSeg_t * twoD;
vfdSeg_t * twoE;
vfdSeg_t * twoF;
vfdSeg_t * twoG;
vfdSeg_t * twoDP;
// Tube 3
vfdSeg_t * threeA;
vfdSeg_t * threeB;
vfdSeg_t * threeC;
vfdSeg_t * threeD;
vfdSeg_t * threeE;
vfdSeg_t * threeF;
vfdSeg_t * threeG;
vfdSeg_t * threeDP;
// Tube 4
vfdSeg_t * fourA;
vfdSeg_t * fourB;
vfdSeg_t * fourC;
vfdSeg_t * fourD;
vfdSeg_t * fourE;
vfdSeg_t * fourF;
vfdSeg_t * fourG;
vfdSeg_t * fourDP;
// Tube 5
vfdSeg_t * fiveA;
vfdSeg_t * fiveB;
vfdSeg_t * fiveC;
vfdSeg_t * fiveD;
vfdSeg_t * fiveE;
vfdSeg_t * fiveF;
vfdSeg_t * fiveG;
vfdSeg_t * fiveDP;
// Tube 6
vfdSeg_t * sixA;
vfdSeg_t * sixB;
vfdSeg_t * sixC;
vfdSeg_t * sixD;
vfdSeg_t * sixE;
vfdSeg_t * sixF;
vfdSeg_t * sixG;
vfdSeg_t * sixDP;

/*
 * Tube declarations
 */
vfdTube_t * hoursOne;
vfdTube_t * hoursTwo;
vfdTube_t * minutesOne;
vfdTube_t * minutesTwo;
vfdTube_t * secondsOne;
vfdTube_t * secondsTwo;

/*
 * Mode stuff
 */
//Mode state = clock;

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// Create 6 * 8 segments
	makeVFDsegs();

	// Create Tubes
	hoursOne = create_vfd_tube(oneA, oneB, oneC, oneD, oneE, oneF, oneG, oneDP);
    hoursTwo = create_vfd_tube(twoA, twoB, twoC, twoD, twoE, twoF, twoG, twoDP);
    minutesOne = create_vfd_tube(threeA, threeB, threeC, threeD, threeE, threeF, threeG, threeDP);
    minutesTwo = create_vfd_tube(fourA, fourB, fourC, fourD, fourE, fourF, fourG, fourDP);
    secondsOne = create_vfd_tube(fiveA, fiveB, fiveC, fiveD, fiveE, fiveF, fiveG, fiveDP);
    secondsTwo = create_vfd_tube(sixA, sixB, sixC, sixD, sixE, sixF, sixG, sixDP);

    // configure peripherals
	configure_rtc();
//	configure_uart();
//	configure_buttons;

	P1->DIR |= BIT0;
	P1->OUT &= ~BIT0;

	while(1) {
//	    if( !P1->IN & BIT5 ) {
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
            update_tubes(hours, minutes, seconds);
//	    }
//	    if( P1->IN & BIT5 ) {
//	        uint16_t i;
//	        for(i = 0; i < 25000; i++);
//	        P1->OUT ^= BIT0;
//	    }
	}
}
