/*
 * Jack Danielski
 * VFD Tube Clock
 *
 * tubes.c
 *
 *  Contains functions that utilize shift registers to update
 *  the 7-segment displays
 */

#include "msp.h"
#include <stdint.h>
#include "tubes.h"

/*
 * Binary values that will display the corresponding
 * numbers on the seven segment displays after being
 * loaded to the shift register
 */
#define ZERO        0b00111111; // 0x3F
#define ONE         0b00110000; // 0x30
#define TWO         0b01011011; // 0x5B
#define THREE       0b01001111; // 0x4F
#define FOUR        0b01100110; // 0x66
#define FIVE        0b01101101; // 0x6D
#define SIX         0b01111101; // 0x7D
#define SEVEN       0b00000111; // 0x07
#define EIGHT       0b01111111; // 0x7F
#define NINE        0b01101111; // 0x6F

/*
 *  Pin numbers on Port 4 that connect to the
 *  serial pin of the shift register
 */
#define TUBE_1_SER      2   // Hours 1
#define TUBE_2_SER      3   // Hours 2
#define TUBE_3_SER      4   // Minutes 1
#define TUBE_4_SER      5   // Minutes 2
#define TUBE_5_SER      6   // Seconds 1
#define TUBE_6_SER      7   // Seconds 2


/* 1-6 for select tubes, 7 for all */
void disableOutput(uint8_t target) {
    if(target == 7) {
        P3->OUT |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    }
    else {
        P3->OUT |= BIT(target-1);
    }
}

/* Enable the output from the shift register */
void enableOutput(uint8_t target) {
    if(target == 7) {
        P3->OUT &= ~( BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 );
    }
    else {
        P3->OUT &= ~BIT(target-1);
    }
}

/* Pulse clock pin to shift a bit in the shift registers  */
void pulseClock() {
    P4->OUT |= BIT0;
    P4->OUT &= ~(BIT0);
}

/* Configures the shift registers to be used */
void configure_shift_pins() {
    P4->SEL0 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->SEL0 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);

    P4->SEL1 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->SEL1 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);

    // set to output
    P4->DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);

    // set all outputs low
    P4->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
}


/* returns a value that will show the input value
 * in the uint8_t on 7-seg display
 */
uint8_t decToSevSeg(uint8_t value) {
    switch(value) {
        case 0: return ZERO;
        case 1: return ONE;
        case 2: return TWO;
        case 3: return THREE;
        case 4: return FOUR;
        case 5: return FIVE;
        case 6: return SIX;
        case 7: return SEVEN;
        case 8: return EIGHT;
        case 9: return NINE;
        default: return ZERO;
    }
}

/* sets a pin for an assigned tube high or low */
void assignPin(uint8_t tube, uint8_t val) {
    if(val == 1)
        P4->OUT |= BIT(tube+1);
    else
        P4->OUT &= ~BIT(tube+1);
}

/* updates only the two tubes related to seconds */
void updateSeconds(uint8_t decSecs) {
    uint8_t segSecsOne = decToSevSeg(decSecs / 10);
    uint8_t segSecsTwo = decToSevSeg(decSecs % 10);

    P4->OUT &= ~(BIT1); // latch (!SRCLR), set P4.1 low

    shiftOut(5, segSecsOne);
    shiftOut(6, segSecsTwo);

    P4->OUT |= BIT1;    // set latch (!SRCLR0) high again
}

/* updates only the two tubes related to minutes */
void updateMinutes(uint8_t decMins, uint8_t decSecs) {
    uint8_t segMinsOne = decToSevSeg(decMins / 10);
    uint8_t segMinsTwo = decToSevSeg(decMins % 10);
    uint8_t segSecsOne = decToSevSeg(decSecs / 10);
    uint8_t segSecsTwo = decToSevSeg(decSecs % 10);

    P4->OUT &= ~(BIT1); // latch (!SRCLR), set P4.1 low

    // write the values to the tubes
    shiftOut(3, segMinsOne);
    shiftOut(4, segMinsTwo);
    shiftOut(5, segSecsOne);
    shiftOut(6, segSecsTwo);

    P4->OUT |= BIT1;    // set latch (!SRCLR0) high again
}

/* updates hours, minutes, and seconds all at once */
void updateTime(uint8_t decHrs, uint8_t decMins, uint8_t decSecs) {
    P4->OUT &= ~(BIT1); // latch (!SRCLR), set P4.1 low

    uint8_t segHrsOne = decToSevSeg(decHrs / 10);
    uint8_t segHrsTwo = decToSevSeg(decHrs % 10);
    uint8_t segMinsOne = decToSevSeg(decMins / 10);
    uint8_t segMinsTwo = decToSevSeg(decMins % 10);
    uint8_t segSecsOne = decToSevSeg(decSecs / 10);
    uint8_t segSecsTwo = decToSevSeg(decSecs % 10);

    P4->OUT &= ~(BIT1); // latch (!SRCLR), set P4.1 low

    // write the values to the tubes
    shiftOut(1, segHrsOne);
    shiftOut(2, segHrsTwo);
    shiftOut(3, segMinsOne);
    shiftOut(4, segMinsTwo);
    shiftOut(5, segSecsOne);
    shiftOut(6, segSecsTwo);

    P4->OUT |= BIT1;    // set latch (!SRCLR0) high again
}

/* places a value in the shift register */
void shiftOut(uint8_t tubeNumber, uint8_t val) {
    uint8_t i;
    for(i = 0; i < 8; i++) {
        assignPin(tubeNumber+1, (val & (1 << i))); // P4.2 (SER) for tube 1
//        P4->OUT ^= BIT2; // test value
        pulseClock(); // P4.0
    }
}
