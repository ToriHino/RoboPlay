//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// ROL: AdLib Visual Composer player

#ifndef __ROL_H
#define __ROL_H

#define ROL_FILE_DATA     0x8000
#define ROL_MAX_FILE_SIZE 0x4000

#define ROL_MAJOR_VERSION 0
#define ROL_MINOR_VERSION 4

#define ROL_NR_CHANNELS 11

#define MAXNOTES             96
#define NRSTEPPITCH          25 // 25 steps within a half-tone for pitch bend
#define NUMSEMITONESINOCTAVE 12

#define MAXVOLUME   0x7F
#define MAXTICKBEAT 60

#define SILENCENOTE -12

#define BASSDRUMCHANNEL  6
#define SNAREDRUMCHANNEl 7
#define TOMTOMCHANNEL    8
#define TOMTOMNOTE       24
#define TOMTOMTOSNARE    7    // 7 half-tones between voice 7 & 8
#define SNARENOTE        TOMTOMNOTE + TOMTOMTOSNARE


#define OPL2_WaveCtrlBaseAddress      = 0x01 // Test LSI / Enable waveform control
#define OPL2_AaMultiBaseAddress       = 0x20 // Amp Mod / Vibrato / EG type / Key Scaling / Multiple
#define OPL2_KSLTLBaseAddress         = 0x40 // Key scaling level / Operator output level
#define OPL2_ArDrBaseAddress          = 0x60 // Attack Rate / Decay Rate
#define OPL2_SlrrBaseAddress          = 0x80 // Sustain Level / Release Rate
#define OPL2_FreqLoBaseAddress        = 0xA0 // Frequency (low 8 bits)
#define OPL2_KeyOnFreqHiBaseAddress   = 0xB0 // Key On / Octave / Frequency (high 2 bits)
#define OPL2_AmVibRhythmBaseAddress   = 0xBD // AM depth / Vibrato depth / Rhythm control
#define OPL2_FeedConBaseAddress       = 0xC0 // Feedback strength / Connection type
#define OPL2_WaveformBaseAddress      = 0xE0 // Waveform select

#define OPL2_EnableWaveformSelectMask = 0x20
#define OPL2_KeyOnMask                = 0x20
#define OPL2_RhythmMask               = 0x20
#define OPL2_KSLMask                  = 0xC0
#define OPL2_TLMask                   = 0x3F
#define OPL2_TLMinLevel               = 0x3F
#define OPL2_FNumLSBMask              = 0xFF
#define OPL2_FNumMSBMask              = 0x03
#define OPL2_FNumMSBShift             = 0x08
#define OPL2_BlockNumberShift         = 0x02

static uint8_t const NoteOctave[MAXNOTES] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

static uint8_t const NoteIndex[MAXNOTES] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

static uint16_t const FNumNotes[NRSTEPPITCH][NUMSEMITONESINOCTAVE] =
{
    { 343, 364, 385, 408, 433, 459, 486, 515, 546, 579, 614, 650 },
    { 344, 365, 387, 410, 434, 460, 488, 517, 548, 581, 615, 652 },
    { 345, 365, 387, 410, 435, 461, 489, 518, 549, 582, 617, 653 },
    { 346, 366, 388, 411, 436, 462, 490, 519, 550, 583, 618, 655 },
    { 346, 367, 389, 412, 437, 463, 491, 520, 551, 584, 619, 657 },
    { 347, 368, 390, 413, 438, 464, 492, 522, 553, 586, 621, 658 },
    { 348, 369, 391, 415, 439, 466, 493, 523, 554, 587, 622, 660 },
    { 349, 370, 392, 415, 440, 467, 495, 524, 556, 589, 624, 661 },
    { 350, 371, 393, 416, 441, 468, 496, 525, 557, 590, 625, 663 },
    { 351, 372, 394, 417, 442, 469, 497, 527, 558, 592, 627, 665 },
    { 351, 372, 395, 418, 443, 470, 498, 528, 559, 593, 628, 666 },
    { 352, 373, 396, 419, 444, 471, 499, 529, 561, 594, 630, 668 },
    { 353, 374, 397, 420, 445, 472, 500, 530, 562, 596, 631, 669 },
    { 354, 375, 398, 421, 447, 473, 502, 532, 564, 597, 633, 671 },
    { 355, 376, 398, 422, 448, 474, 503, 533, 565, 599, 634, 672 },
    { 356, 377, 399, 423, 449, 475, 504, 534, 566, 600, 636, 674 },
    { 356, 378, 400, 424, 450, 477, 505, 535, 567, 601, 637, 675 },
    { 357, 379, 401, 425, 451, 478, 506, 537, 569, 603, 639, 677 },
    { 358, 379, 402, 426, 452, 479, 507, 538, 570, 604, 640, 679 },
    { 359, 380, 403, 427, 453, 480, 509, 539, 571, 606, 642, 680 },
    { 360, 381, 404, 428, 454, 481, 510, 540, 572, 607, 643, 682 },
    { 360, 382, 405, 429, 455, 482, 511, 541, 574, 608, 645, 683 },
    { 361, 383, 406, 430, 456, 483, 512, 543, 575, 610, 646, 685 },
    { 362, 384, 407, 431, 457, 484, 513, 544, 577, 611, 648, 687 },
    { 363, 385, 408, 432, 458, 485, 514, 545, 578, 612, 649, 688 }
};

static uint8_t const DrumOpTable[4] = 
    { 0x14, 0x12, 0x15, 0x11 };

typedef struct
{
    uint16_t majorVersion;
    uint16_t minorVersion;
    char     signature[40];
    uint16_t tickBeat;
    uint16_t beatMeasure;
    uint16_t scaleY;
    uint16_t scaleX;
    char     reserved;
    boolean  isMelodic;
    uint16_t counters[45];
    uint8_t  filler[38];
} ROL_HEADER;

typedef struct
{
    uint16_t atTick;
    float    multiplier;
} ROL_TEMPO;

typedef struct
{
    char       trackName[15];
    float      basicTempo;
    uint16_t   nEvents;
    ROL_TEMPO* tempoEvents; 
} ROL_TEMPO_TRACK;

typedef struct
{
    uint16_t note;
    uint16_t duration;
} ROL_NOTE;

typedef struct
{
    char      trackName[15];
    uint16_t  nTicks;
    ROL_NOTE* noteEvents;
} ROL_VOICE_TRACK;

typedef struct 
{
    uint16_t atTick;
    char     instName[9];
    uint8_t  filler;
    uint16_t unknown;
} ROL_TIMBRE;

typedef struct
{
    char        trackName[15];
    uint16_t    nEvents;
    ROL_TIMBRE* timbreEvents;
} ROL_TIMBRE_TRACK;

typedef struct
{
    uint16_t atTick;
    float    volume;
} ROL_VOLUME;

typedef struct
{
    char        trackName[15];
    uint16_t    nEvents;
    ROL_VOLUME* volumeEvents;
} ROL_VOLUME_TRACK;

typedef struct
{
    uint16_t atTick;
    float    pitch;
} ROL_PITCH;

typedef struct
{
    char       trackName[15];
    uint16_t   nEvents;
    ROL_PITCH* pitchEvents;
} ROL_PITCH_TRACK;

typedef struct
{
    boolean eventStatusNoteEnd;
    boolean eventStatusInstrEnd;
    boolean eventStatusVolumeEnd;
    boolean eventStatusPitchEnd;
  
    int16_t noteDuration;
    int16_t currentNoteDuration;
  
    uint16_t currentNote;
    uint16_t nextInstrumentEvent;
    uint16_t nextVolumeEvent;
    uint16_t nextPitchEvent;
  
    boolean forceNote;

    ROL_VOICE_TRACK*  voiceTrack;
    ROL_TIMBRE_TRACK* timbreTrack;
    ROL_VOLUME_TRACK* volumeTrack;
    ROL_PITCH_TRACK*  pitchTrack;
} ROL_VOICE_DATA;

ROL_HEADER       rolHeader;
ROL_VOICE_DATA   voiceData[ROL_NR_CHANNELS];
ROL_TEMPO_TRACK* tempoTrack;

uint16_t currTick;
uint16_t timeOfLastNote;
uint16_t nextTempoEvent;

float refresh;

void FT_SetRefresh(const float multiplier);
void FT_UpdateVoice(const uint8_t voice);
void FT_SendInstData(const uint8_t voice, char* instName);
void FT_SetVolume(const uint8_t voice, const uint8_t volume);
void FT_SetNote(const uint8_t voice, const uint16_t note);
void FT_SetPitch(const uint8_t voice, const float pitch);


void FT_SetNoteMelodic(const uint8_t voice, const uint16_t note);
void FT_SetNotePercussive(const uint8_t voice, const uint16_t note);

#endif