/*
 *
 * Jack Danielski
 * VFD Tube Clock
 *  tubes.h
 *
 */

#ifndef TUBES_H_
#define TUBES_H_

#include "msp.h"
#include "rtc.h"

#define ONEA    11
#define ONEB    12
#define ONEC    13
#define ONED    14
#define ONEE    15
#define ONEF    16
#define ONEG    17
#define ONEDP   18
#define TWOA    20
#define TWOB    21
#define TWOC    22
#define TWOD    23
#define TWOE    24
#define TWOF    25
#define TWOG    26
#define TWODP   27
#define THREEA  30
#define THREEB  31
#define THREEC  32
#define THREED  33
#define THREEE  34
#define THREEF  35
#define THREEG  36
#define THREEDP 37
#define FOURA   40
#define FOURB   41
#define FOURC   42
#define FOURD   43
#define FOURE   44
#define FOURF   45
#define FOURG   46
#define FOURDP  47
#define FIVEA   50
#define FIVEB   51
#define FIVEC   52
#define FIVED   53
#define FIVEE   54
#define FIVEF   55
#define FIVEG   56
#define FIVEDP  57
#define SIXA    60
#define SIXB    61
#define SIXC    62
#define SIXD    63
#define SIXE    64
#define SIXF    65
#define SIXG    66
#define SIXDP   67


typedef struct {
    uint8_t port;       // use '1' to signify port 1
    uint8_t bitnum;     // bit 0-9 of Port X
} vfdSeg_t;

typedef struct {
    vfdSeg_t a;
    vfdSeg_t b;
    vfdSeg_t c;
    vfdSeg_t d;
    vfdSeg_t e;
    vfdSeg_t f;
    vfdSeg_t g;
    vfdSeg_t dp;
} vfdTube_t;

vfdSeg_t * create_vfd_seg( uint8_t pb );//uint8_t port, uint8_t bitnumber);

void makeVFDsegs();

void update_seg_on(vfdSeg_t * seg);

void update_seg_off(vfdSeg_t * seg);

vfdTube_t * create_vfd_tube();

void sevensegConversion(vfdTube_t * current, uint8_t value);

void dec_to_seven_seg(vfdTube_t * firstVFD, vfdTube_t * secondVFD, uint8_t hms);

void update_tubes(uint8_t hours, uint8_t minutes, uint8_t seconds);

#endif
