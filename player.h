//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC

#ifndef __PLAYER_H
#define __PLAYER_H

#define TRACK_DATA       0x8000
#define PAGE_SIZE        0x4000

#define ROBO_PLAYER_BASE 0x4000

typedef struct
{
    char signature[8];
    byte majorVersion;
    byte minorVersion;

    // To be implemented by player
    boolean (*FT_load_ptr)(char* fileName);
    boolean (*FT_update_ptr)();
    void (*FT_rewind_ptr)(int8_t subsong);
    float (*FT_getRefresh_ptr)();

    uint8_t (*FT_getSubSongs_ptr)();

    char* (*FT_getPlayerInfo_ptr)();
    char* (*FT_getTitle_ptr)();
    char* (*FT_getAuthor_ptr)();
    char* (*FT_getDescription_ptr)();

    // Callbacks to RoboPlay environment
    void (*FT_Open_ptr)(char *name);
    uint16_t (*FT_Read_ptr)(void *buf, uint16_t nbytes);
    void (*FT_Close_ptr)();

    byte (*FT_AllocateSegment_ptr)();
    void (*FT_SetSegment_ptr)(uint8_t segment);

    void (*FT_SetRefresh_ptr)(float refresh);

    void (*FT_WriteOpl1_ptr)(uint8_t addr, uint8_t value);
    void (*FT_WriteOpl2_ptr)(uint8_t addr, uint8_t value);
} ROBO_PLAYER_HEADER;

extern boolean FT_load(char* fileName);
extern boolean FT_update();
extern void FT_rewind(int8_t subsong);
extern float FT_getRefresh();
extern uint8_t FT_getSubSongs();

#endif