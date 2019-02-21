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

/*  7 - S E G M E N T   V A L U E S  */

/* Binary values that will display the corresponding
 * numbers on the seven segment displays after being
 * loaded to the shift register  */
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
#define LETTER_F    0b01101011; // 0x##
#define DEGREES     0b01101011; // 0x6B


/*  S H I F T   R E G I S T E R   F U N C T I O N S  */

/* 1-6 for select tubes, 7 for all */
void disable_output(uint8_t target) {
    if(target == 7)
        P3->OUT |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
    else
        P3->OUT |= BIT(target-1);
}

/* Enable the output from the shift register */
void enable_output(uint8_t tube) {
    if(tube == 7)
        P3->OUT &= ~( BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 );
    else
        P3->OUT &= ~BIT(tube-1);
}

/* Pulse clock pin to shift a bit in the shift registers  */
void pulse_clock() {
    P4->OUT |= BIT0;    // SRCLK
    P6->OUT &= ~(BIT1); // RCLK

    P4->OUT &= ~(BIT0); // SRCLK
    P6->OUT |= BIT1;    // RCLK
}

/* Configures the shift registers to be used */
void configure_shift_pins() {
    P6->SEL0 &= ~(BIT1);
    P4->SEL0 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->SEL0 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);

    P6->SEL1 &= ~(BIT1);
    P4->SEL1 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->SEL1 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);

    // set to output
    P6->DIR |= (BIT1);
    P4->DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->DIR |= (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);

    // set all outputs low
    P6->OUT &= ~(BIT1);
    P4->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P3->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5);
}


/* returns a value that will show the input value
 * in the uint8_t on 7-seg display */
uint8_t dec_to_sev_seg(uint8_t value) {
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
void assign_pin(uint8_t tube, uint8_t val) {
    if(val != 0)
        P4->OUT |= BIT(tube+1);
    else
        P4->OUT &= ~BIT(tube+1);
}

/* updates only the two tubes related to seconds */
void update_seconds(uint8_t decSecs) {
    uint8_t segSecsOne = dec_to_sev_seg(decSecs / 10);
    uint8_t segSecsTwo = dec_to_sev_seg(decSecs % 10);

    P4->OUT |= (BIT1);              // latch (!SRCLR), set P4.1 low

    // write the values to shift registers
    shift_out(5, segSecsOne);
    shift_out(6, segSecsTwo);

    P4->OUT &= ~(BIT1);             // set latch (!SRCLR0) low again
}

/* updates only the two tubes related to minutes */
void update_minutes(uint8_t decMins, uint8_t decSecs) {
    uint8_t segMinsOne = dec_to_sev_seg(decMins / 10);
    uint8_t segMinsTwo = dec_to_sev_seg(decMins % 10);
    uint8_t segSecsOne = dec_to_sev_seg(decSecs / 10);
    uint8_t segSecsTwo = dec_to_sev_seg(decSecs % 10);

    P4->OUT |= (BIT1);              // latch (!SRCLR), set P4.1 high

    // write the values to shift registers
    shift_out(3, segMinsOne);
    shift_out(4, segMinsTwo);
    shift_out(5, segSecsOne);
    shift_out(6, segSecsTwo);

    P4->OUT &= ~(BIT1);             // set latch (!SRCLR0) high again
}

/* updates hours, minutes, and seconds */
void update_time(uint8_t decHrs, uint8_t decMins, uint8_t decSecs) {
    P4->OUT &= ~(BIT1);             // latch (!SRCLR), set P4.1 low

    // convert from decimal to seven segment binary
    uint8_t segHrsOne = dec_to_sev_seg(decHrs / 10);
    uint8_t segHrsTwo = dec_to_sev_seg(decHrs % 10);
    uint8_t segMinsOne = dec_to_sev_seg(decMins / 10);
    uint8_t segMinsTwo = dec_to_sev_seg(decMins % 10);
    uint8_t segSecsOne = dec_to_sev_seg(decSecs / 10);
    uint8_t segSecsTwo = dec_to_sev_seg(decSecs % 10);

    P4->OUT |= (BIT1);              // latch (!SRCLR), set P4.1 high

    // write the values to the tubes
    shift_out(1, segHrsOne);
    shift_out(2, segHrsTwo);
    shift_out(3, segMinsOne);
    shift_out(4, segMinsTwo);
    shift_out(5, segSecsOne);
    shift_out(6, segSecsTwo);

    P4->OUT &= ~(BIT1);             // set latch (!SRCLR0) low again
}


/* updates the temperature to display '  ##oF' */
void update_temperature(uint8_t temperature) {
    uint8_t temperatureOne = dec_to_sev_seg(temperature / 10);
    uint8_t temperatureTwo = dec_to_sev_seg(temperature % 10);
    uint8_t degrees = DEGREES;
    uint8_t letter_f = LETTER_F;

    P4->OUT |= (BIT1);              // latch (!SRCLR), set P4.1 high

    // write to shift registers
    shift_out(1, 0);                //  blank
    shift_out(2, 0);                //  blank
    shift_out(3, temperatureOne);   //  #
    shift_out(4, temperatureTwo);   //  #
    shift_out(5, degrees);          //  degrees
    shift_out(6, letter_f);         //  letter_f

    P4->OUT &= ~(BIT1);             // set latch (!SRCLR0) low again
}


/* places a value in the shift register */
void shift_out(uint8_t tubeNumber, uint8_t val) {
    uint8_t i;
    for(i = 0; i < 8; i++) {
        assign_pin(tubeNumber, (val & (1 << i)));   // P4.2 (SER) for tube 1
        pulse_clock();                              // P4.0
    }
    P6->OUT &= ~(BIT1);
}
