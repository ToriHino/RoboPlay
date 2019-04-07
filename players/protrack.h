//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// Generic Protracker-based formats player

#ifndef __PROTRACKER_H
#define __PROTRACKER_H

typedef struct
{
    uint8_t data[11];
    uint8_t arpstart;
    uint8_t arpspeed;
    uint8_t arppos;
    uint8_t arpspdcnt;
    uint8_t misc;
    int8_t  slide;
} Instrument;

typedef struct 
{
    uint8_t note;
    uint8_t command;
    uint8_t inst;
    uint8_t param1;
    uint8_t param2;
} Track;

typedef struct
{
    uint16_t freq, nextfreq;
    uint8_t  oct, vol1, vol2, inst, fx, info1, info2, key, nextoct;
    uint8_t  note, portainfo, vibinfo1, vibinfo2, arppos, arpspdcnt;
    int8_t   trigger;
} Channel;

#endif