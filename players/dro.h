//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// DRO: DOSBox Raw OPL v2.0 player

#ifndef __DRO_H
#define __DRO_H

#define HARDWARETYPE_OPL2       0
#define HARDWARETYPE_OPL3       1
#define HARDWARETYPE_DUAL_OPL2  2

#define READ_BUFFER         0xC000
#define READ_BUFFER_SIZE    0x1000

typedef struct 
{
    char signature[8];
    uint16_t versionMajor;
    uint16_t versionMinor;
    
    uint32_t lengthPairs;
    uint32_t lengthMS;
    uint8_t hardwareType;
    uint8_t format;
    uint8_t compression;
    uint8_t shortDelayCode;
    uint8_t longDelayCode;
    uint8_t codeMapLength;   
} DRO2_HEADER;

DRO2_HEADER dro2Header;
uint8_t codeMap[256];

uint8_t segmentList[256];
uint8_t currentSegment;

uint16_t delayCounter;
uint32_t indexPtr;

uint8_t* songdata;

#endif