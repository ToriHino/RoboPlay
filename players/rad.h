//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// RAD: Reality ADLib Tracker player

#ifndef __RAD_H
#define __RAD_H

#define READ_BUFFER         0xC000
#define READ_BUFFER_SIZE    0x1000

#define RAD_MAX_INST    31
#define RAD_MAX_PATTERN 32
#define RAD_MAX_CHANNEL 9
#define RAD_MAX_LINE    64

#define RAD_CMD_PORTAMENTO_UP   0x1
#define RAD_CMD_PORTAMENTO_DOWN 0x2
#define RAD_CMD_TONE_SLIDE      0x3
#define RAD_CMD_TONE_VOL_SLIDE  0x5
#define RAD_CMD_VOL_SLIDE       0xA
#define RAD_CMD_SET_VOL         0xC
#define RAD_CMD_JUMP_TO_LINE    0xD
#define RAD_CMD_SET_SPEED       0xF

const uint16_t NoteFreq[] = { 0x16b,0x181,0x198,0x1b0,0x1ca,0x1e5,0x202,0x220,0x241,0x263,0x287,0x2ae };
const uint8_t  op_table[] = { 0x00, 0x01, 0x02, 0x08, 0x09, 0x0a, 0x10, 0x11, 0x12 };

typedef struct
{
    char id[16];
    uint8_t version;
    uint8_t flags;
} RAD_HEADER;

typedef struct 
{
    uint8_t carrier_23;
    uint8_t modulator_20;
    uint8_t carrier_43;
    uint8_t modulator_40;
    uint8_t carrier_63;
    uint8_t modulator_60;
    uint8_t carrier_83;
    uint8_t modulator_80;
    uint8_t both_c0;
    uint8_t carrier_e3;
    uint8_t modulator_e0;
} RAD_INST;

typedef struct
{
    int8_t   portSlide;
    int8_t   volSlide;
    uint16_t toneSlideFreq;
    uint8_t  toneSlideOct;
    uint8_t  toneSlideSpeed;
    int8_t   toneSlideDir;
} RAD_EFFECTS;

typedef struct 
{
    RAD_INST*   instrument;
    uint8_t     volume;
    uint8_t     keyFlags;
    uint16_t    currFreq;
    uint8_t     currOctave;
    RAD_EFFECTS fx;
} RAD_CHANNEL;

enum {
    fKeyOn   = 1 << 0,
    fKeyOff  = 1 << 1,
    fKeyedOn = 1 << 2,
};

RAD_HEADER* header;
RAD_INST*   instruments[RAD_MAX_INST];
uint8_t*    patterns;
RAD_CHANNEL channels[RAD_MAX_CHANNEL];

uint8_t offset;

uint8_t hertz;
float speed;

uint8_t  orderLength;
uint8_t* orderList;

uint8_t* pattern;

uint8_t  speedCnt;
uint8_t  order;
uint8_t  line;
uint8_t  masterVol;

int8_t   lineJump;

int8_t   noteNum;
int8_t   octaveNum;
uint8_t  instNum;
uint8_t  effectNum;
uint8_t  param;

uint8_t* filedata;

uint8_t* FT_GetPattern();
void     FT_PlayLine();
void     FT_ResetFX(uint8_t channel);
void     FT_ContinueFX(uint8_t channel);
boolean  FT_UnpackNote();
void     FT_PlayNote(uint8_t channel);
void     FT_SetVolume(uint8_t channel, uint8_t volume);
void     FT_Portamento(uint8_t channel, int8_t amount, boolean toneSlide);
void     FT_PlayNoteOPL(uint8_t channel);
void     FT_GetSlideDir(uint8_t channel);

#endif