/*
 *
 * Jack Danielski
 * VFD Tube Clock
 *  tubes.c
 *
 */

#include "msp.h"
#include "stdlib.h"
#include "tubes.h"

/*
 * Segment declarations
 */
// Tube 1
extern vfdSeg_t * oneA;
extern vfdSeg_t * oneB;
extern vfdSeg_t * oneC;
extern vfdSeg_t * oneD;
extern vfdSeg_t * oneE;
extern vfdSeg_t * oneF;
extern vfdSeg_t * oneG;
extern vfdSeg_t * oneDP;
// Tube 2
extern vfdSeg_t * twoA;
extern vfdSeg_t * twoB;
extern vfdSeg_t * twoC;
extern vfdSeg_t * twoD;
extern vfdSeg_t * twoE;
extern vfdSeg_t * twoF;
extern vfdSeg_t * twoG;
extern vfdSeg_t * twoDP;
// Tube 3
extern vfdSeg_t * threeA;
extern vfdSeg_t * threeB;
extern vfdSeg_t * threeC;
extern vfdSeg_t * threeD;
extern vfdSeg_t * threeE;
extern vfdSeg_t * threeF;
extern vfdSeg_t * threeG;
extern vfdSeg_t * threeDP;
// Tube 4
extern vfdSeg_t * fourA;
extern vfdSeg_t * fourB;
extern vfdSeg_t * fourC;
extern vfdSeg_t * fourD;
extern vfdSeg_t * fourE;
extern vfdSeg_t * fourF;
extern vfdSeg_t * fourG;
extern vfdSeg_t * fourDP;
// Tube 5
extern vfdSeg_t * fiveA;
extern vfdSeg_t * fiveB;
extern vfdSeg_t * fiveC;
extern vfdSeg_t * fiveD;
extern vfdSeg_t * fiveE;
extern vfdSeg_t * fiveF;
extern vfdSeg_t * fiveG;
extern vfdSeg_t * fiveDP;
// Tube 6
extern vfdSeg_t * sixA;
extern vfdSeg_t * sixB;
extern vfdSeg_t * sixC;
extern vfdSeg_t * sixD;
extern vfdSeg_t * sixE;
extern vfdSeg_t * sixF;
extern vfdSeg_t * sixG;
extern vfdSeg_t * sixDP;

/*
 * Tube declarations
 */
extern vfdTube_t * hoursOne;
extern vfdTube_t * hoursTwo;
extern vfdTube_t * minutesOne;
extern vfdTube_t * minutesTwo;
extern vfdTube_t * secondsOne;
extern vfdTube_t * secondsTwo;


vfdSeg_t * create_vfd_seg( uint8_t pb ) {
//      uint8_t port, uint8_t bitnumber) {
//    if( !port )
//        return NULL;
//
//    else {
//        vfdSeg_t * temp = malloc(sizeof(vfdSeg_t));
//        temp->port = port;
//        temp->bitnum = bitnumber;
//        return temp;
//    }
    vfdSeg_t * temp = malloc( sizeof(vfdSeg_t) );
    temp->port = pb / 10;
    temp->bitnum = pb % 10;
    return temp;
}

void makeVFDsegs() {
    oneA = create_vfd_seg(ONEA);
    oneB = create_vfd_seg(ONEB);
    oneC = create_vfd_seg(ONEC);
    oneD = create_vfd_seg(ONED);
    oneE = create_vfd_seg(ONEE);
    oneF = create_vfd_seg(ONEF);
    oneG = create_vfd_seg(ONEG);
    oneDP = create_vfd_seg(ONEDP);
    twoA = create_vfd_seg(TWOA);
    twoB = create_vfd_seg(TWOB);
    twoC = create_vfd_seg(TWOC);
    twoD = create_vfd_seg(TWOD);
    twoE = create_vfd_seg(TWOE);
    twoF = create_vfd_seg(TWOF);
    twoG = create_vfd_seg(TWOG);
    twoDP = create_vfd_seg(TWODP);
    threeA = create_vfd_seg(THREEA);
    threeB = create_vfd_seg(THREEB);
    threeC = create_vfd_seg(THREEC);
    threeD = create_vfd_seg(THREED);
    threeE = create_vfd_seg(THREEE);
    threeF = create_vfd_seg(THREEF);
    threeG = create_vfd_seg(THREEG);
    threeDP = create_vfd_seg(THREEDP);
    fourA = create_vfd_seg(FOURA);
    fourB = create_vfd_seg(FOURB);
    fourC = create_vfd_seg(FOURC);
    fourD = create_vfd_seg(FOURD);
    fourE = create_vfd_seg(FOURE);
    fourF = create_vfd_seg(FOURF);
    fourG = create_vfd_seg(FOURG);
    fourDP = create_vfd_seg(FOURDP);
    fiveA = create_vfd_seg(FIVEA);
    fiveB = create_vfd_seg(FIVEB);
    fiveC = create_vfd_seg(FIVEC);
    fiveD = create_vfd_seg(FIVED);
    fiveE = create_vfd_seg(FIVEE);
    fiveF = create_vfd_seg(FIVEF);
    fiveG = create_vfd_seg(FIVEG);
    fiveDP = create_vfd_seg(FIVEDP);
    sixA = create_vfd_seg(SIXA);
    sixB = create_vfd_seg(SIXB);
    sixC = create_vfd_seg(SIXC);
    sixD = create_vfd_seg(SIXD);
    sixE = create_vfd_seg(SIXE);
    sixF = create_vfd_seg(SIXF);
    sixG = create_vfd_seg(SIXG);
    sixDP = create_vfd_seg(SIXDP);
}

void update_seg_on(vfdSeg_t * seg) {
    switch( seg->port ) {
    case 1:
        P1->OUT |= BIT(seg->bitnum);
        break;
    case 2:
        P2->OUT |= BIT(seg->bitnum);
        break;
    case 3:
        P3->OUT |= BIT(seg->bitnum);
        break;
    case 4:
        P4->OUT |= BIT(seg->bitnum);
        break;
    case 5:
        P5->OUT |= BIT(seg->bitnum);
        break;
    case 6:
        P6->OUT |= BIT(seg->bitnum);
        break;
    case 7:
        P7->OUT |= BIT(seg->bitnum);
        break;
    case 8:
        P8->OUT |= BIT(seg->bitnum);
        break;
    case 9:
        P9->OUT |= BIT(seg->bitnum);
        break;
    case 10:
        P10->OUT |= BIT(seg->bitnum);
        break;
    }
}

void update_seg_off(vfdSeg_t * seg) {
    switch( seg->port ) {
    case 1:
        P1->OUT &= ~BIT(seg->bitnum);
        break;
    case 2:
        P2->OUT &= ~BIT(seg->bitnum);
        break;
    case 3:
        P3->OUT &= ~BIT(seg->bitnum);
        break;
    case 4:
        P4->OUT &= ~BIT(seg->bitnum);
        break;
    case 5:
        P5->OUT &= ~BIT(seg->bitnum);
        break;
    case 6:
        P6->OUT &= ~BIT(seg->bitnum);
        break;
    case 7:
        P7->OUT &= ~BIT(seg->bitnum);
        break;
    case 8:
        P8->OUT &= ~BIT(seg->bitnum);
        break;
    case 9:
        P9->OUT &= ~BIT(seg->bitnum);
        break;
    case 10:
        P10->OUT &= ~BIT(seg->bitnum);
        break;
    }
}

vfdTube_t * create_vfd_tube(vfdSeg_t * sa, vfdSeg_t * sb, vfdSeg_t * sc, vfdSeg_t * sd,
                            vfdSeg_t * se, vfdSeg_t * sf, vfdSeg_t * sg, vfdSeg_t * sdp)
{

    if( !sa->port || !sb->port || !sc->port || !sd->port || !se->port || !sf->port || !sg->port || !sdp->port )
        return NULL;

    if( !sa->bitnum || !sb->bitnum || !sc->bitnum || !sd->bitnum || !se->bitnum || !sf->bitnum || !sg->bitnum || !sdp->bitnum )
        return NULL;

    else {
        vfdTube_t * temp = malloc(sizeof(vfdTube_t));
        temp->a = *sa;
        temp->b = *sb;
        temp->c = *sc;
        temp->d = *sd;
        temp->e = *se;
        temp->f = *sf;
        temp->g = *sg;
        temp->dp = *sdp;
        return temp;
    }
}

void sevensegConversion(vfdTube_t * current, uint8_t value) {
    switch( value ) {
    case 0:
        update_seg_on( &current->a );
        update_seg_on( &current->b );
        update_seg_on( &current->c );
        update_seg_on( &current->d );
        update_seg_on( &current->e );
        update_seg_on( &current->f );
        update_seg_off( &current->g );
        update_seg_off( &current->dp );
        break;
    case 1:
        update_seg_off( &current->a );
        update_seg_off( &current->b );
        update_seg_off( &current->c );
        update_seg_off( &current->d );
        update_seg_on( &current->e );
        update_seg_on( &current->f );
        update_seg_off( &current->g );
        update_seg_off( &current->dp );
        break;
    case 2:
        update_seg_on( &current->a );
        update_seg_on( &current->b );
        update_seg_off( &current->c );
        update_seg_on( &current->d );
        update_seg_on( &current->e );
        update_seg_off( &current->f );
        update_seg_on( &current->g );
        update_seg_off( &current->dp );
        break;
    case 3:
        update_seg_on( &current->a );
        update_seg_on( &current->b );
        update_seg_on( &current->c );
        update_seg_on( &current->d );
        update_seg_off( &current->e );
        update_seg_off( &current->f );
        update_seg_on( &current->g );
        update_seg_off( &current->dp );
        break;
    case 4:
        update_seg_off( &current->a );
        update_seg_on( &current->b );
        update_seg_on( &current->c );
        update_seg_off( &current->d );
        update_seg_off( &current->e );
        update_seg_on( &current->f );
        update_seg_on( &current->g );
        update_seg_off( &current->dp );
        break;
    case 5:
        update_seg_on( &current->a );
        update_seg_off( &current->b );
        update_seg_on( &current->c );
        update_seg_on( &current->d );
        update_seg_off( &current->e );
        update_seg_on( &current->f );
        update_seg_on( &current->g );
        update_seg_off( &current->dp );
        break;
    case 6:
        update_seg_on( &current->a );
        update_seg_off( &current->b );
        update_seg_on( &current->c );
        update_seg_on( &current->d );
        update_seg_on( &current->e );
        update_seg_on( &current->f );
        update_seg_on( &current->g );
        update_seg_off( &current->dp );
        break;
    case 7:
        update_seg_on( &current->a );
        update_seg_on( &current->b );
        update_seg_on( &current->c );
        update_seg_off( &current->d );
        update_seg_off( &current->e );
        update_seg_off( &current->f );
        update_seg_off( &current->g );
        update_seg_off( &current->dp );
        break;
    case 8:
        update_seg_on( &current->a );
        update_seg_on( &current->b );
        update_seg_on( &current->c );
        update_seg_on( &current->d );
        update_seg_on( &current->e );
        update_seg_on( &current->f );
        update_seg_on( &current->g );
        update_seg_off( &current->dp );
        break;
    case 9:
        update_seg_on( &current->a );
        update_seg_on( &current->b );
        update_seg_on( &current->c );
        update_seg_on( &current->d );
        update_seg_off( &current->e );
        update_seg_on( &current->f );
        update_seg_on( &current->g );
        update_seg_off( &current->dp );
        break;
    }
}

// converts Decimal number to two 7seg numbers
void dec_to_seven_seg(vfdTube_t * firstVFD, vfdTube_t * secondVFD, uint8_t hms) {
    // find the values that firstVFD and secondsVFD should display
    uint8_t ones = hms % 10;
    uint8_t tens = hms / 10;

    // convert values to 7seg
    sevensegConversion(firstVFD, tens);
    sevensegConversion(secondVFD, ones);
}

void update_tubes(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    dec_to_seven_seg(hoursOne, hoursTwo, hours);
    dec_to_seven_seg(minutesOne, minutesTwo, minutes);
    dec_to_seven_seg(secondsOne, secondsTwo, seconds);

}
