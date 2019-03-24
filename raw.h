//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// RAW: RAW OPL capture player

#ifndef __RAW_H
#define __RAW_H

#define READ_BUFFER         0xC000
#define READ_BUFFER_SIZE    0x1000

#define RAW_DELAY     0x00
#define RAW_CONTROL   0x02

#define RAW_CONTROL_TYPE_CLOCK      0x00
#define RAW_CONTROL_TYPE_LOW_OPL    0x01
#define RAW_CONTROL_TYPE_HIGH_OPL   0x02

typedef struct
{
    char signature[8];
    uint16_t clock;
} RAW_HEADER;

typedef struct 
{
    uint8_t param;
    uint8_t command;
} RAW_DATA;

RAW_HEADER rawHeader;

uint8_t segmentList[256];
uint8_t currentSegment;

uint16_t delayCounter;
uint16_t speed;

RAW_DATA* songdata;
uint16_t pos;

boolean highOPL;

#endif