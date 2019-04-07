//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// SOP: Note OPL3 Music Sequencer player

#ifndef __SOP_H
#define __SOP_H

#define READ_BUFFER       0xC000
#define READ_BUFFER_SIZE  0x1000

#define VOLUME_TABLE_BASE SEGMENT_BASE
#define VOLUME_TABLE_SIZE 0x2000

#define VOLUME_SEGMENT     0
#define INSTRUMENT_SEGMENT 0

#define INSTRUMENTS_BASE  (VOLUME_TABLE_BASE + VOLUME_TABLE_SIZE)
#define SOP_EVNT_BASE     SEGMENT_BASE

#define SOP_DEF_TEMPO 120

#define SOP_MAX_TRACK   24
#define SOP_MAX_VOL     127

#define SOP_CHAN_NONE   0
#define SOP_CHAN_4OP    1
#define SOP_CHAN_2OP    2

#define INSTRUMENT_TYPE_MELODY_4OP 0
#define INSTRUMENT_TYPE_MELODY_2OP 1
#define INSTRUMENT_TYPE_BASS_DRUM  6
#define INSTRUMENT_TYPE_SNARE_DRUM 7
#define INSTRUMENT_TYPE_TOM_TOM    8
#define INSTRUMENT_TYPE_CYMBAL     9
#define INSTRUMENT_TYPE_HI_HAT     10
#define INSTRUMENT_TYPE_UNUSED     12

#define INSTRUMENT_DATA_SIZE_2OP  11
#define INSTRUMENT_DATA_SIZE_4OP  22

#define SOP_EVNT_SPEC   1   /* Special event */
#define SOP_EVNT_NOTE   2   /* Note event */
#define SOP_EVNT_TEMPO  3   /* Tempo event */
#define SOP_EVNT_VOL    4   /* Volume event */
#define SOP_EVNT_PITCH  5   /* Pitch event */
#define SOP_EVNT_INST   6   /* Instrument event */
#define SOP_EVNT_PAN    7   /* Panning event */
#define SOP_EVNT_MVOL   8   /* Master volume event */

/* percussive voice numbers: */
#define BD      6
#define SD      7
#define TOM     8
#define CYMB    9
#define HIHAT  10

#define MAX_VOLUME          0x7f
#define LOG2_VOLUME         7       /* log2( MAX_VOLUME) */

#define MID_C               60      /* MIDI standard mid C */

#define SOP_TOM_PITCH       36      /* best frequency, in range of 0 to 95 */
#define TOM_TO_SD           7       /* 7 half-tones between voice 7 & 8 */
#define SOP_SD_PITCH        (SOP_TOM_PITCH + TOM_TO_SD)

#define NB_NOTES            96      /* 8 octave of 12 notes */
#define OCTAVE              12      /* half-tone by octave */
#define NB_STEP_PITCH       32      /* 32 steps between two half-tones */
#define LOG_NB_STEP_PITCH   5       /* LOG2( NB_STEP_PITCH ) */
#define NB_TABLE_DEMI_TON   OCTAVE
#define TABLE_SIZE          (NB_STEP_PITCH * NB_TABLE_DEMI_TON)

#define maxVoices       20
#define YMB_SIZE        80

typedef struct
{
    char signature[7];
    uint8_t majorVersion;
    uint8_t minorVersion;
    uint8_t padding_1;
    char fileName[13];
    char title[31];
    uint8_t percussive;
    uint8_t padding_2;
    uint8_t tickBeat;
    uint8_t padding_3;
    uint8_t beatMeasure;
    uint8_t basicTempo;
    char comment[13];
    uint8_t nTracks;
    uint8_t nInsts;
    uint8_t padding_4;
} SOP_HEADER;

typedef struct
{
    uint8_t instType;
    char shortName[8];
    char longName[19];
    uint8_t instData[22];
} SOP_INST;

typedef struct
{
    uint16_t numEvents;
    uint16_t curEvent;

    uint8_t  startSegment;
    uint8_t* startData;
    uint8_t  currentSegment;
    uint8_t* currentData;

    uint16_t ticks;
    uint16_t dur;
    uint16_t counter;
} SOP_TRK;

SOP_HEADER sopHeader;
SOP_INST*  instruments;
SOP_TRK    tracks[SOP_MAX_TRACK+1];

uint16_t tickBeat;
uint8_t chanMode[SOP_MAX_TRACK+1];

uint8_t volume[SOP_MAX_TRACK+1];
uint8_t lastvol[SOP_MAX_TRACK+1];
uint8_t masterVolume;

const uint16_t fNumTbl[TABLE_SIZE]={
    0x0159, 0x015A, 0x015A, 0x015B, 0x015C, 0x015C, 0x015D, 0x015D, 0x015E, 0x015F, 0x015F, 0x0160,
    0x0161, 0x0161, 0x0162, 0x0162, 0x0163, 0x0164, 0x0164, 0x0165, 0x0166, 0x0166, 0x0167, 0x0168,
    0x0168, 0x0169, 0x016A, 0x016A, 0x016B, 0x016C, 0x016C, 0x016D, 0x016E, 0x016E, 0x016F, 0x016F,
    0x0170, 0x0171, 0x0171, 0x0172, 0x0173, 0x0174, 0x0174, 0x0175, 0x0176, 0x0176, 0x0177, 0x0178,
    0x0178, 0x0179, 0x017A, 0x017A, 0x017B, 0x017C, 0x017C, 0x017D, 0x017E, 0x017E, 0x017F, 0x0180,
    0x0180, 0x0181, 0x0182, 0x0183, 0x0183, 0x0184, 0x0185, 0x0185, 0x0186, 0x0187, 0x0187, 0x0188,
    0x0189, 0x018A, 0x018A, 0x018B, 0x018C, 0x018C, 0x018D, 0x018E, 0x018F, 0x018F, 0x0190, 0x0191,
    0x0191, 0x0192, 0x0193, 0x0194, 0x0194, 0x0195, 0x0196, 0x0197, 0x0197, 0x0198, 0x0199, 0x019A,
    0x019A, 0x019B, 0x019C, 0x019D, 0x019D, 0x019E, 0x019F, 0x019F, 0x01A0, 0x01A1, 0x01A2, 0x01A3,
    0x01A3, 0x01A4, 0x01A5, 0x01A6, 0x01A6, 0x01A7, 0x01A8, 0x01A9, 0x01A9, 0x01AA, 0x01AB, 0x01AC,
    0x01AC, 0x01AD, 0x01AE, 0x01AF, 0x01B0, 0x01B0, 0x01B1, 0x01B2, 0x01B3, 0x01B3, 0x01B4, 0x01B5,
    0x01B6, 0x01B7, 0x01B7, 0x01B8, 0x01B9, 0x01BA, 0x01BB, 0x01BB, 0x01BC, 0x01BD, 0x01BE, 0x01BF,
    0x01BF, 0x01C0, 0x01C1, 0x01C2, 0x01C3, 0x01C3, 0x01C4, 0x01C5, 0x01C6, 0x01C7, 0x01C8, 0x01C8,
    0x01C9, 0x01CA, 0x01CB, 0x01CC, 0x01CD, 0x01CD, 0x01CE, 0x01CF, 0x01D0, 0x01D1, 0x01D2, 0x01D2,
    0x01D3, 0x01D4, 0x01D5, 0x01D6, 0x01D7, 0x01D7, 0x01D8, 0x01D9, 0x01DA, 0x01DB, 0x01DC, 0x01DD,
    0x01DD, 0x01DE, 0x01DF, 0x01E0, 0x01E1, 0x01E2, 0x01E3, 0x01E4, 0x01E4, 0x01E5, 0x01E6, 0x01E7,
    0x01E8, 0x01E9, 0x01EA, 0x01EB, 0x01EB, 0x01EC, 0x01ED, 0x01EE, 0x01EF, 0x01F0, 0x01F1, 0x01F2,
    0x01F3, 0x01F3, 0x01F4, 0x01F5, 0x01F6, 0x01F7, 0x01F8, 0x01F9, 0x01FA, 0x01FB, 0x01FC, 0x01FD,
    0x01FE, 0x01FE, 0x01FF, 0x0200, 0x0201, 0x0202, 0x0203, 0x0204, 0x0205, 0x0206, 0x0207, 0x0208,
    0x0209, 0x020A, 0x020B, 0x020B, 0x020C, 0x020D, 0x020E, 0x020F, 0x0210, 0x0211, 0x0212, 0x0213,
    0x0214, 0x0215, 0x0216, 0x0217, 0x0218, 0x0219, 0x021A, 0x021B, 0x021C, 0x021D, 0x021E, 0x021F,
    0x0220, 0x0221, 0x0222, 0x0223, 0x0224, 0x0225, 0x0226, 0x0227, 0x0228, 0x0229, 0x022A, 0x022B,
    0x022C, 0x022D, 0x022E, 0x022F, 0x0230, 0x0231, 0x0232, 0x0233, 0x0234, 0x0235, 0x0236, 0x0237,
    0x0238, 0x0239, 0x023A, 0x023B, 0x023C, 0x023D, 0x023E, 0x023F, 0x0240, 0x0241, 0x0242, 0x0243,
    0x0244, 0x0245, 0x0246, 0x0247, 0x0248, 0x0249, 0x024B, 0x024C, 0x024D, 0x024E, 0x024F, 0x0250,
    0x0251, 0x0252, 0x0253, 0x0254, 0x0255, 0x0256, 0x0257, 0x0258, 0x025A, 0x025B, 0x025C, 0x025D,
    0x025E, 0x025F, 0x0260, 0x0261, 0x0262, 0x0263, 0x0265, 0x0266, 0x0267, 0x0268, 0x0269, 0x026A,
    0x026B, 0x026C, 0x026D, 0x026F, 0x0270, 0x0271, 0x0272, 0x0273, 0x0274, 0x0275, 0x0276, 0x0278,
    0x0279, 0x027A, 0x027B, 0x027C, 0x027D, 0x027E, 0x0280, 0x0281, 0x0282, 0x0283, 0x0284, 0x0285,
    0x0287, 0x0288, 0x0289, 0x028A, 0x028B, 0x028C, 0x028E, 0x028F, 0x0290, 0x0291, 0x0292, 0x0294,
    0x0295, 0x0296, 0x0297, 0x0298, 0x029A, 0x029B, 0x029C, 0x029D, 0x029E, 0x02A0, 0x02A1, 0x02A2,
    0x02A3, 0x02A4, 0x02A6, 0x02A7, 0x02A8, 0x02A9, 0x02AB, 0x02AC, 0x02AD, 0x02AE, 0x02B0, 0x02B1
};

const uint8_t SlotX[maxVoices * 2]={
    0, 1, 2, 8, 9, 10, 16, 17, 18,  0,  0, 0, 1, 2, 8, 9, 10, 16, 17, 18,
    0, 1, 2, 8, 9, 10, 16, 20, 18, 21, 17, 0, 1, 2, 8, 9, 10, 16, 17, 18
};

const uint8_t VolReg[11 * 2]={
    0x43, 0x44, 0x45, 0x4B, 0x4C, 0x4D, 0x53, 0x54, 0x55, 0, 0,
    0x43, 0x44, 0x45, 0x4B, 0x4C, 0x4D, 0x53, 0x54, 0x52, 0x55, 0x51
};

const uint8_t MOD12[OCTAVE * 11]={
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

const uint8_t DIV12[OCTAVE * 8]={
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

boolean percussion; /* percussion mode parameter */

uint8_t* VolumeTable; /* pre-calculated Volume Table */

char voiceNote[20];    /* pitch of last note-on of each voice */
char voiceKeyOn[20];   /* state of keyOn bit of each voice */
uint8_t vPitchBend[20];   /* current pitch bend of each voice */
uint8_t Ksl[20];          /* KSL value for Slot 1 */
uint8_t Ksl2[20];         /* KSL value for Slot 2 */
uint8_t Ksl2V[20];        /* Parallel connection? */
uint8_t VoiceVolume[20], OP_MASK;
uint8_t ymbuf[ 2 * YMB_SIZE ];
uint8_t OP4[20];
uint8_t Stereo[22];

uint16_t interval;

void FT_ExecuteCommand(uint8_t track);
void FT_SetTempo(uint8_t tempo);

boolean FT_LoadSOPInstruments();
boolean FT_LoadSOPTrackData();

void FT_Set4OPMode(uint8_t channel, uint8_t value);
void FT_SetPercussionMode(boolean mode);

void FT_InitVolumeTable();

void FT_WriteOpl1(uint8_t addr, uint8_t value);
void FT_WriteOpl2(uint8_t addr, uint8_t value);

void FT_NoteOn(uint8_t channel, uint8_t pitch);
void FT_NoteOff(uint8_t channel);
void FT_SetFreq(uint8_t voice, uint8_t note, int16_t pitch, uint8_t keyOn);
void FT_SetVoiceVolume(uint8_t chan, uint8_t vol);
void FT_SendIns(uint16_t base_addr, uint8_t* value, boolean mode);
void FT_SetVoiceTimbre(uint8_t chan, byte* array);
void FT_SetStereoPAN(uint8_t chan, uint8_t value);
void FT_SetVoicePitch(uint8_t chan, uint8_t pitch);

uint8_t FT_GetTrackData(uint8_t track);

#endif