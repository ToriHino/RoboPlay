//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC

#ifndef __PLAYER_H
#define __PLAYER_H

#define ROBO_PLAYER_BASE 0x4000

#define SEGMENT_BASE     0x8000
#define SEGMENT_SIZE     0x4000

#undef __DEBUG

typedef struct
{
    char signature[8];
    uint8_t majorVersion;
    uint8_t minorVersion;

    // To be implemented by the player
    boolean (*RP_Load)(char* fileName);
    boolean (*RP_Update)();
    void (*RP_Rewind)(int8_t subsong);
    float (*RP_GetRefresh)();

    uint8_t (*RP_GetSubSongs)();

    char* (*RP_GetPlayerInfo)();
    char* (*RP_GetTitle)();
    char* (*RP_GetAuthor)();
    char* (*RP_GetDescription)();

    // Callbacks to RoboPlay environment
    void (*RP_Open)(char *name);
    uint16_t (*RP_Read)(void *buf, uint16_t nbytes);
    void (*RP_Close)();
    boolean (*RP_Exists)(char *name);

    uint8_t (*RP_AllocateSegment)();
    void (*RP_SetSegment)(uint8_t segment);
    void (*RP_SetDataSegment)();

    void (*RP_UpdateRefresh)();

    void (*RP_WriteOpl1)(uint8_t addr, uint8_t value);
    void (*RP_WriteOpl2)(uint8_t addr, uint8_t value);
    void (*RP_WriteWave)(uint8_t addr, uint8_t value);
    void (*RP_StartWaveDataWrite)();
    void (*RP_WriteWaveData)(uint8_t value);

    uint8_t (*RP_ReadOpl)();
    uint8_t (*RP_ReadWave)(uint8_t addr);

#ifdef __DEBUG
    void (*RP_DebugString)(char* s);
    void (*RP_DebugHex8)(uint8_t b);
    void (*RP_DebugHex16)(uint16_t b);
#endif
} ROBO_PLAYER_INTERFACE;

#endif