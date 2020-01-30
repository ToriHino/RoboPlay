//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// VGM: Video Game Music player

#ifndef __VGM_H
#define __VGM_H

#define READ_BUFFER         0xC000
#define READ_BUFFER_SIZE    0x1000

#define VGM_GZIP_MIN    8       // minimum size for GZip header
#define VGM_HEADER_MIN  84      // minimum size for header (till YM3812 clock field)
#define VGM_HEADER_ID   "Vgm "  // header ident
#define OFFSET_EOF      0x04    // End Of File offset
#define OFFSET_GD3      0x14    // GD3 tag offset
#define OFFSET_LOOP     0x1C    // Loop offset
#define OFFSET_DATA     0x34    // VGM data offset
#define OFFSET_LOOPBASE 0x7E    // Loop base offset
#define OFFSET_LOOPMOD  0x7F    // Loop modifier offset
#define VGM_DUAL_BIT    0x40000000  // VGM flag for dual-chip feature
#define GD3_HEADER_ID   "Gd3 "  // GD3 tag header ident

#define CMD_OPL2        0x5A    // YM3812, write value <dd> to register <aa>
#define CMD_OPL1        0x5B    // YM3526, write value <dd> to register <aa>
#define CMD_OPL         0x5C    // Y8950, write value <dd> to register <aa>
#define CMD_OPL3_PORT0  0x5E    // YMF262 port 0, write value <dd> to register <aa>
#define CMD_OPL3_PORT1  0x5F    // YMF262 port 1, write value <dd> to register <aa>
#define CMD_WAIT        0x61    // Wait <n> samples, <n> can range from 0 to 65535 (approx 1.49 seconds)
#define CMD_WAIT_735    0x62    // wait 735 samples (60th of a second)
#define CMD_WAIT_882    0x63    // wait 882 samples (50th of a second)
#define CMD_DATA_END    0x66    // end of sound data
#define CMD_WAIT_N      0x70    // wait <n+1> samples, <n> can range from 0 to 15
#define CMD_OPL2_2ND    0xAA    // YM3812 chip 2, write value <dd> to register <aa>
#define CMD_OPL4        0xD0    // YMF278B port pp, write value dd to register aa

#define VGM_FREQUENCY   44100.0 // VGM base sample frequency
#define VGM_REPLAY_FREQ 735     // VGM replay frequency
#define VGM_STEP_VALUE  60      // VGM_FREQUENCY / VGM_REPLAY_FREQ

typedef enum
{
    VGM_DEVICE_ID_YM3526,
    VGM_DEVICE_ID_Y8950,
    VGM_DEVICE_ID_YM3812,
    VGM_DEVICE_ID_YM3812_DUAL,
    VGM_DEVICE_ID_YMF262,
    VGM_DEVICE_ID_YMF278B,

    VGM_DEVICE_ID_CARD
} VGM_DEVICE_ID;

typedef struct
{
    VGM_DEVICE_ID id;
    char* description;
} VGM_DEVICES;

const VGM_DEVICES deviceDescriptions[VGM_DEVICE_ID_CARD] =
{
    { VGM_DEVICE_ID_YM3526,      "YM3526 OPL capture" },
    { VGM_DEVICE_ID_Y8950,       "Y8950 MSX-AUDIO capture" },
    { VGM_DEVICE_ID_YM3812,      "YM3812 OPL2 capture" },
    { VGM_DEVICE_ID_YM3812_DUAL, "YM3812 OPL2 Dual capture" },
    { VGM_DEVICE_ID_YMF262,      "YMF262 OPL3 capture" },
    { VGM_DEVICE_ID_YMF278B,     "YMF278B OPL4 capture" }
};

typedef struct 
{
    uint32_t vgmIdent;
    uint32_t eofOffset;
    uint32_t version;
    uint32_t sn76589Clock;

    uint32_t ym2413Clock;
    uint32_t gd3Offset;
    uint32_t totalNrSamples;
    uint32_t loopOffset;

    uint32_t loopNrSamples;
    uint32_t rate;
    uint8_t  snFeedback;
    uint16_t snw;
    uint8_t  sf;
    uint32_t ym2612Clock;

    uint32_t ym2151Clock;
    uint32_t vgmDataOffset;
    uint32_t segaPCMClock;
    uint32_t spcmInterface;

    uint32_t rf5c68Clock;
    uint32_t ym2203Clock;
    uint32_t ym2608Clock;
    uint32_t ym2610bClock;

    uint32_t ym3812Clock;
    uint32_t ym3526Clock;
    uint32_t y8950Clock;
    uint32_t ymf262Clock;

    uint32_t ymf278bClock;
    uint32_t ymf271Clock;
    uint32_t ymz280bClock;
    uint32_t rf5c164Clock;

    uint32_t pwmClock;
    uint32_t ay8910Clock;
    uint8_t  ayType;
    uint8_t  ayFlags;
    uint8_t  ym2203AyFlags;
    uint8_t  ym2608AyFlags;
    uint8_t  volumeModifier;
    uint8_t  reserved;
    uint8_t  loopBase;
    uint8_t  loopModifier;
} VGM_HEADER;

uint8_t segmentList[256];
uint8_t currentSegment;

uint32_t indexPtr;
uint32_t vgmSize;

uint16_t delayCounter;

VGM_HEADER vgmHeader;

VGM_DEVICE_ID deviceType;

uint32_t clock;

uint8_t* songdata;

uint8_t GetVgmData();

#endif