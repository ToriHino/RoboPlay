//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// D00: EdLib packed module player

#ifndef __D00_H
#define __D00_H

#define D00_FILE_DATA     SEGMENT_BASE
#define D00_MAX_FILE_SIZE SEGMENT_SIZE

typedef struct 
{
    char id[6];
    uint8_t type;
    uint8_t version;
    uint8_t speed;
    int8_t subsongs;
    uint8_t soundcard;
    char songname[32];
    char author[32];
    char dummy[32];

    uint16_t tpoin;
    uint16_t seqptr;
    uint16_t instptr;
    uint16_t infoptr;
    uint16_t spfxptr;
    uint16_t endmark;
} D00_HEADER;

typedef struct
{
    uint8_t version;
    uint8_t speed;
    int8_t subsongs;

    uint16_t tpoin;
    uint16_t seqptr;
    uint16_t instptr;
    uint16_t infoptr;
    uint16_t lpulptr;
    uint16_t endmark;   
} D00_HEADER_1;

typedef struct
{
    uint16_t *order,ordpos,pattpos,del,speed,rhcnt,key,freq,inst, spfx,ispfx,irhcnt;
    int16_t   transpose,slide,slideval,vibspeed;
    uint8_t   seqend,vol,vibdepth,fxdel,modvol,cvol,levpuls, frameskip,nextnote,note,ilevpuls,trigger,fxflag;
} CHANNEL;

typedef struct 
{
    uint8_t data[11], tunelev, timer, sr, dummy[2];
} INSTS;

typedef struct
{
    uint16_t instnr;
    int8_t   halfnote;
    uint8_t  modlev;
    int8_t   modlevadd;
    uint8_t  duration;
    uint16_t ptr;
} SPFX;

typedef struct
{
    uint8_t level;
    int8_t  voladd;
    uint8_t duration, ptr;
} LEVPULS;

typedef struct
{
    uint16_t ptr[9];
    uint8_t  volume[9],dummy[5];  
} TPOIN;

CHANNEL  channel[9];
INSTS*   inst;
SPFX*    spfx;
LEVPULS* levpuls;

uint8_t  songend, version, cursubsong;
uint8_t* datainfo;

uint16_t* seqptr;

D00_HEADER*   header;
D00_HEADER_1* header1;

uint8_t* filedata;

void FT_SetVolume(uint8_t chan);
void FT_SetFreq(uint8_t chan);
void FT_SetInst(uint8_t chan);
void FT_PlayNote(uint8_t chan);
void FT_Vibrato(uint8_t chan);

#endif