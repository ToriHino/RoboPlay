//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// MUS: AdLib MIDI Music File player

#ifndef __MUS_H
#define __MUS_H

#define READ_BUFFER         0xC000
#define READ_BUFFER_SIZE    0x1000
#define INSTRUMENTS         0x6000
#define INSTRUMENTS_SIZE    0x2000

#define nbLocParam          14
#define MAX_VOICES          11

#define OVERFLOW_TICKS      240
#define MAX_SEC_DELAY       10.0

#define SYSTEM_XOR_BYTE     0xF0
#define EOX_BYTE            0xF7
#define OVERFLOW_BYTE       0xF8
#define STOP_BYTE           0xFC

#define NOTE_OFF_BYTE           0x80
#define NOTE_ON_BYTE            0x90
#define AFTER_TOUCH_BYTE        0xA0
#define CONTROL_CHANGE_BYTE     0xB0
#define PROG_CHANGE_BYTE        0xC0
#define CHANNEL_PRESSURE_BYTE   0xD0
#define PITCH_BEND_BYTE         0xE0

#define ADLIB_CTRL_BYTE 0x7F    /* for System exclusive */
#define TEMPO_CTRL_BYTE 0

 /* percussive voice numbers: */
#define BD              6
#define SD              7
#define TOM             8
#define CYMB            9
#define HIHAT           10

#define MAX_VOICES      11
#define MAX_VOLUME      0x7f
#define MAX_PITCH       0x3fff
#define MID_PITCH       0x2000

#define MID_C           60          /* MIDI standard mid C */
#define CHIP_MID_C      48          /* sound chip mid C */

#define TOM_PITCH   24              /* best frequency, in range of 0 to 95 */
#define TOM_TO_SD   7               /* 7 half-tones between voice 7 & 8 */
#define SD_PITCH    (TOM_PITCH + TOM_TO_SD)

#define NR_STEP_PITCH   25          /* 25 steps within a half-tone for pitch bend */

#define prmKsl          0
#define prmMulti        1
#define prmFeedBack     2       /* use for opr. 0 only */
#define prmAttack       3
#define prmSustain      4
#define prmStaining     5       /* Sustaining ... */
#define prmDecay        6
#define prmRelease      7
#define prmLevel        8
#define prmAm           9
#define prmVib          10
#define prmKsr          11
#define prmFm           12          /* use for opr. 0 only */
#define prmWaveSel      13          /* wave select */

typedef struct 
{
    uint8_t  majorVersion;
    uint8_t  minorVersion;
    int32_t  tuneId;
    char     tuneName[30];
    uint8_t  tickBeat;
    uint8_t  beatMeasure;
    int32_t  totalTick;
    int32_t  dataSize;
    int32_t  nrCommand;
    uint8_t  filler[8];
    uint8_t  soundMode;
    uint8_t  pitchBRange;
    uint16_t basicTempo;
    uint8_t  filler2[8];
} MUS_HEADER;

typedef struct 
{
    uint8_t  verMajor;
    uint8_t  verMinor;
    char     signature[6];
    uint16_t numUsed;
    uint16_t numInstruments;
    uint32_t offsetName;
    uint32_t offsetData;
    uint8_t  pad[8];
} BNK_HEADER;

typedef struct
{
    char name[9];
} IMS_INSTRUMENT;

typedef struct
{
    uint16_t index;
    uint8_t  flags;
    char     name[9];
} BNK_INSTRUMENT_HEADER;

typedef struct 
{
    uint8_t ksl;
    uint8_t multiple;
    uint8_t feedback;
    uint8_t attack;
    uint8_t sustain;
    uint8_t eg;
    uint8_t decay;
    uint8_t releaseRate;
    uint8_t totalLevel;
    uint8_t am;
    uint8_t vib;
    uint8_t ksr;
    uint8_t con;
} OPLREGS;

typedef struct 
{
    uint8_t iPercussive;
    uint8_t iVoiceNum;
    OPLREGS oplModulator;
    OPLREGS oplCarrier;
    uint8_t iModWaveSel;
    uint8_t iCarWaveSel;
} BNK_INSTRUMENT;

typedef struct
{
    uint8_t         segment;
    BNK_INSTRUMENT *bnkInst;
} INSTRUMENT;

MUS_HEADER  musHeader;
BNK_HEADER* bnkHeader;

uint8_t segmentList[256];
uint8_t currentSegment;
uint8_t instSegment;

INSTRUMENT inst[256];

uint8_t* songdata;
uint8_t* bankdata;

float    Timer;
uint32_t Pos;
uint32_t Counter;
uint32_t Ticks;
uint8_t  Status;
uint8_t  Volume[MAX_VOICES];

int PitchRangeStep;

uint8_t paramSlot[18][nbLocParam];

uint8_t percBits;               /* control bits of percussive voices */
uint8_t notePitch[MAX_VOICES];  /* pitch of last note-on of each voice */
uint8_t voiceKeyOn[MAX_VOICES]; /* state of keyOn bit of each voice */
uint8_t amDepth;                /* chip global parameters .. */
uint8_t vibDepth;               /* ... */

uint8_t slotRelVolume[18];      /* relative volume of slots */

uint8_t noteDIV12[96];          /* table of (0..95) DIV 12 */
uint8_t noteMOD12[96];          /* table of (0..95) MOD 12 */

uint16_t  fNumNotes[NR_STEP_PITCH][12];
int       halfToneOffset[MAX_VOICES];
uint16_t* fNumFreqPtr[MAX_VOICES];

void     FT_FindInstruments();
uint32_t FT_GetTicks();
uint8_t  FT_GetData();

void FT_ExecuteCommand();
void FT_SetTempo(uint16_t tempo, uint8_t tickBeat);

void FT_SetMode();
void FT_SetSlotParam(uint8_t slot, int16_t* param, uint8_t waveSel);
void FT_SetCharSlotParam(uint8_t slot, uint8_t* cParam, uint8_t waveSel);
void FT_InitSlotParams();
void FT_SetFreq(uint8_t voice, int pitch, uint8_t keyOn);
void FT_SoundChut(uint8_t voice);
void FT_NoteOn(uint8_t voice, int16_t pitch);
void FT_NoteOff(uint8_t voice);
void FT_SetVoiceVolume(uint8_t voice, uint8_t vol);
void FT_SetVoiceTimbre(uint8_t voice, uint8_t inst);
void FT_SetVoicePitch(uint8_t voice, uint8_t pitch);

void FT_SndSAmVibRhythm();
void FT_SndSKslLevel(uint8_t slot);
void FT_SndSetAllPrm(uint8_t slot);

#endif