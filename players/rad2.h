//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// RAD: Reality ADLib Tracker player

#ifndef __RAD2_H
#define __RAD2_H

#define RAD_MAX_TRACK      100
#define RAD_MAX_CHANNEL    9
#define RAD_MAX_LINE       64
#define RAD_MAX_RIFF_TRACK 10
#define RAD_MAX_INST       127

#define RAD_CMD_PORTAMENTO_UP   0x1
#define RAD_CMD_PORTAMENTO_DOWN 0x2
#define RAD_CMD_TONE_SLIDE      0x3
#define RAD_CMD_TONE_VOL_SLIDE  0x5
#define RAD_CMD_VOL_SLIDE       0xA
#define RAD_CMD_SET_VOL         0xC
#define RAD_CMD_JUMP_TO_LINE    0xD
#define RAD_CMD_SET_SPEED       0xF
#define RAD_CMD_IGNORE          ('I' - 55)
#define RAD_CMD_MULTIPLIER      ('M' - 55)
#define RAD_CMD_RIFF            ('R' - 55)
#define RAD_CMD_TRANSPOSE       ('T' - 55)
#define RAD_CMD_FEEDBACK        ('U' - 55)
#define RAD_CMD_VOLUME          ('V' - 55)

const uint8_t NoteSize[] = { 0, 2, 1, 3, 1, 3, 2, 4 };
const uint16_t ChanOffsets3[9] = { 0, 1, 2, 0x100, 0x101, 0x102, 6, 7, 8 };              // OPL3 first channel
const uint16_t Chn2Offsets3[9] = { 3, 4, 5, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108 };  // OPL3 second channel
const uint16_t NoteFreq[] = { 0x16b,0x181,0x198,0x1b0,0x1ca,0x1e5,0x202,0x220,0x241,0x263,0x287,0x2ae };
const uint16_t OpOffsets3[9][4] = {
    {  0x00B, 0x008, 0x003, 0x000  },
    {  0x00C, 0x009, 0x004, 0x001  },
    {  0x00D, 0x00A, 0x005, 0x002  },
    {  0x10B, 0x108, 0x103, 0x100  },
    {  0x10C, 0x109, 0x104, 0x101  },
    {  0x10D, 0x10A, 0x105, 0x102  },
    {  0x113, 0x110, 0x013, 0x010  },
    {  0x114, 0x111, 0x014, 0x011  },
    {  0x115, 0x112, 0x015, 0x012  } 
};
const boolean AlgCarriers[7][4] = {
    {  true, false, false, false  },  // 0 - 2op - op < op
    {  true, true,  false, false  },  // 1 - 2op - op + op
    {  true, false, false, false  },  // 2 - 4op - op < op < op < op
    {  true, false, false, true   },  // 3 - 4op - op < op < op + op
    {  true, false, true,  false  },  // 4 - 4op - op < op + op < op
    {  true, false, true,  true   },  // 5 - 4op - op < op + op + op
    {  true, true,  true,  true   },  // 6 - 4op - op + op + op + op
};


typedef struct
{
    char id[16];
    uint8_t version;
    uint8_t flags;
} RAD_HEADER;

typedef struct 
{
    uint8_t  feedback[2];
    uint8_t  panning[2];
    uint8_t  algorithm;
    uint8_t  detune;
    uint8_t  volume;
    uint8_t  riffSpeed;
    uint8_t* riff;
    uint8_t  operators[4][5];
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
    RAD_EFFECTS fx;
    uint8_t*    track;
    uint8_t*    trackStart;
    uint8_t     line;
    uint8_t     speed;
    uint8_t     speedCnt;
    int8_t      transposeOctave;
    int8_t      transposeNote;
    uint8_t     lastInstrument;
} RAD_RIFF;

typedef struct 
{
    uint8_t     lastInstrument;
    RAD_INST*   instrument;
    uint8_t     volume;
    uint8_t     detuneA;
    uint8_t     detuneB;
    uint8_t     keyFlags;
    uint16_t    currFreq;
    uint8_t     currOctave;
    RAD_EFFECTS fx;
    RAD_RIFF    riff;
    RAD_RIFF    iRiff;
} RAD_CHANNEL;

typedef enum 
{
    SNone, SRiff, SIRiff
} RAD_SOURCE;

enum {
    fKeyOn   = 1 << 0,
    fKeyOff  = 1 << 1,
    fKeyedOn = 1 << 2,
};

RAD_HEADER* Header;
RAD_INST    Instruments[RAD_MAX_INST];
RAD_CHANNEL Channels[RAD_MAX_CHANNEL];
uint8_t*    Tracks[RAD_MAX_TRACK];
uint8_t*    Riffs[RAD_MAX_RIFF_TRACK][RAD_MAX_CHANNEL];

uint8_t Hertz;
float Speed;

uint8_t  OrderListSize;
uint8_t* OrderList;

uint8_t* Track;

uint8_t  SpeedCnt;
uint8_t  Order;
uint8_t  Line;
int8_t   Entrances;
uint8_t  MasterVol;
int8_t   LineJump;
uint8_t  OPL3Regs[512];

int8_t  NoteNum;
int8_t  OctaveNum;
uint8_t  InstNum;
uint8_t  EffectNum;
uint8_t  Param;

uint8_t* FileData;

uint8_t* FT_GetTrack();
void     FT_PlayLine();
void     FT_ResetFX(RAD_EFFECTS* fx);
void     FT_ContinueFX(uint8_t channum, RAD_EFFECTS* fx);
void     FT_TickRiff(uint8_t channum, RAD_RIFF* riff, boolean chan_riff);
boolean  FT_UnpackNote(uint8_t* last_instrument);
void     FT_PlayNote(uint8_t channum, RAD_SOURCE source, uint8_t op);
void     FT_SetVolume(uint8_t channum, uint8_t volume);
uint8_t* FT_SkipToLine(uint8_t* trk, uint8_t linenum, boolean chan_riff);
void     FT_Transpose(int8_t note, int8_t octave) ;
void     FT_Portamento(uint8_t channum, RAD_EFFECTS* fx, int8_t amount, boolean toneSlide);
void     FT_GetSlideDir(uint8_t channum, RAD_EFFECTS* fx);

void     FT_SetOPL3(uint16_t reg, uint8_t val);
uint8_t  FT_GetOPL3(uint16_t reg);
void     FT_PlayNoteOPL3(uint8_t channum);
void     FT_LoadInstrumentOPL3(uint8_t channum);
void     FT_LoadInstMultiplierOPL3(uint8_t channum, uint8_t op, uint8_t mult);
void     FT_LoadInstVolumeOPL3(uint8_t channum, uint8_t op, uint8_t vol);
void     FT_LoadInstFeedbackOPL3(uint8_t channum, uint8_t which, uint8_t fb);

#endif