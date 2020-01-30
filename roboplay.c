//
// RoboPlay
//
// Copyright (C) 2020 by RoboSoft.
//
// Made with FUSION-C in SDCC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/newTypes.h"
#include "fusion-c/header/io.h"
#include "fusion-c/header/vars_msxDos.h"
#include "fusion-c/header/vars_msxSystem.h"
#include "fusion-c/header/rammapper.h"

#include "player.h"
#include "roboplay.h"
#include "opl4.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;
int16_t playerFileHandle;

SEGMENT_INFO segmentList[MAX_NR_SEGMENTS];
uint16_t segmentsFound;
uint8_t segmentAllocationIndex;
uint8_t currentSlot;

uint8_t subsong;

uint8_t dataSegment;

//
// Allocate memory segments
//
void FT_AllocateSegments()
{
    uint16_t i = 0;

    MAPPERINFOBLOCK* pMapperInfoBlock = _GetRamMapperBaseTable();
    SEGMENTSTATUS* pSegmentStatus = 0;

    segmentList[0].allocatedSegmentNumber = Get_PN(PAGE_2);
    segmentList[0].slotAddressOfMapper = pMapperInfoBlock->slot;

    for(i = 1; i < MAX_NR_SEGMENTS; i++)
    {
        pSegmentStatus = AllocateSegment(0, 0x20);

        if(pSegmentStatus->carryFlag) break;
        
        segmentList[i].allocatedSegmentNumber = pSegmentStatus->allocatedSegmentNumber;
        segmentList[i].slotAddressOfMapper    = pSegmentStatus->slotAddressOfMapper;
    }
    segmentsFound = i;
    segmentAllocationIndex = 0;

    printf("\nTotal free memory found: %dK\n\r", segmentsFound * 16);
}

//
// Allocate a free segment
//
uint8_t FT_AllocateSegment()
{
    uint8_t segment;

    if(segmentAllocationIndex >= segmentsFound)
    {
        Exit(_NORAM);
    }

    segment = segmentAllocationIndex++;

    return segment;
}

//
// Set an active segment on page 2
//
void FT_SetSegment(uint8_t segment)
{
    Put_PN(PAGE_2, segmentList[segment].allocatedSegmentNumber);

    if(currentSlot != segmentList[segment].slotAddressOfMapper)
    {
        currentSlot = segmentList[segment].slotAddressOfMapper;

        __asm
        ld a,(_currentSlot)
        ld b,a
        ld hl,#SEGMENT_BASE
        call ENASLT
        __endasm;
    }
}

//
// Set player data segment on page 2
//
void FT_SetDataSegment()
{
    FT_SetSegment(dataSegment);
}


//
// Set refresh rate
//
void FT_SetRefresh()
{
    float refresh = iRoboPlay->RP_GetRefresh();
    if( refresh > 50.0)
    {
        FT_OPL4_FM1_Write(OPL4_TIMER1_COUNT, 255 - (uint8_t)( (1000.0 / refresh) / 0.08));
        FT_OPL4_FM1_Write(OPL4_TIMER_CONTROL, 0x21);
        FT_OPL4_FM1_Write(OPL4_TIMER_CONTROL, 0x80);
    }
    else
    {       
        FT_OPL4_FM1_Write(OPL4_TIMER2_COUNT, 256 - (uint8_t)( (1000.0 / refresh) / 0.320));
        FT_OPL4_FM1_Write(OPL4_TIMER_CONTROL, 0x42);
        FT_OPL4_FM1_Write(OPL4_TIMER_CONTROL, 0x80);
    }
}

//
// Close file
//
void FT_Close()
{
    Close(playerFileHandle);
    playerFileHandle = -1;
}

//
// Open file
//
void FT_Open(char *name)
{
    if(playerFileHandle == -1)
        printf("Loading file: %s", name);
    else
        FT_Close();

    playerFileHandle = Open(name, O_RDONLY);

    if(playerFileHandle == -1)
    {
        Exit(__NOFIL);
    }
}

//
// Read from file
//
uint16_t FT_Read(void *buf, uint16_t nbytes)
{
    return Read(playerFileHandle, buf, nbytes);
}

//
// Check if file exists
//
boolean FT_Exists(char *name)
{
    uint16_t fileHandle = Open(name, O_RDONLY);
    if(fileHandle != -1) Close(fileHandle);

    return (fileHandle != -1);
}

#ifdef __DEBUG
void FT_DebugString(char* s)
{
    printf("%s", s);
}

void FT_DebugHex8(uint8_t b)
{
    printf("%x", b);
}

void FT_DebugHex16(uint16_t b)
{
    printf("%x", b);
}
#endif

//
// Load player data
//
void FT_LoadPlayerData(char* fileName)
{
    uint16_t fH = 0;
    
    printf("Loading data  : %s", fileName);

    // Reserve first segment for player data
    dataSegment = FT_AllocateSegment();

    fH = Open(fileName, O_RDONLY);
    if(fH == -1)
    {
        Exit(__NOFIL);
    }
    Read(fH, (void *)SEGMENT_BASE, SEGMENT_SIZE);
    Close(fH);

    FT_SetSegment(segmentAllocationIndex);

    printf(" ...Done\n\r");
}

//
// Load player module
//
void FT_LoadPlayer(char* fileName)
{
    uint16_t fH = 0;
    int8_t   i = 0;
  
    printf("\nLoading player: %s", fileName);
    fH = Open(fileName, O_RDONLY);
    if(fH == -1)
    {
        Exit(__NOFIL);
    }

    iRoboPlay = (void *)ROBO_PLAYER_BASE;
    Read(fH, iRoboPlay, SEGMENT_SIZE);
    if(strncmp(iRoboPlay->signature,"ROBOPLAY", 8) != 0)
    {
        printf(" ...Error\n\r");
        printf("\n\rNot a RoboPlay player file\n\r");
        Close(fH);
        Exit(__INTER);
    }
    Close(fH);

    printf(" ...Done\n\r");


    i = StrPosStr(fileName, ".PLY");
    fileName[i+1] = 'D';
    fileName[i+2] = 'A';
    fileName[i+3] = 'T';

    if(FT_Exists(fileName))
    {
        FT_LoadPlayerData(fileName);
    }

    printf("%s\n\n\r", iRoboPlay->RP_GetPlayerInfo());

    iRoboPlay->RP_Open =  &FT_Open;
    iRoboPlay->RP_Read =  &FT_Read;
    iRoboPlay->RP_Close = &FT_Close;
    iRoboPlay->RP_Exists = &FT_Exists;
    
    iRoboPlay->RP_AllocateSegment = &FT_AllocateSegment;
    iRoboPlay->RP_SetSegment = &FT_SetSegment;
    iRoboPlay->RP_SetDataSegment = &FT_SetDataSegment;

    iRoboPlay->RP_UpdateRefresh = &FT_SetRefresh;

    iRoboPlay->RP_WriteOpl1 = &FT_OPL4_FM1_Write;
    iRoboPlay->RP_WriteOpl2 = &FT_OPL4_FM2_Write;

    iRoboPlay->RP_WriteWave = &FT_OPL4_WAVE_Write;

    iRoboPlay->RP_ReadOpl = &FT_OPL4_Read;
    iRoboPlay->RP_ReadWave = &FT_OPL4_WAVE_Read;

    iRoboPlay->RP_StartWaveDataWrite = &FT_OPL4_WAVE_StartDataWrite;
    iRoboPlay->RP_WriteWaveData = &FT_OPL4_WAVE_Data;

#ifdef __DEBUG
    iRoboPlay->RP_DebugString = &FT_DebugString;
    iRoboPlay->RP_DebugHex8 = &FT_DebugHex8;
    iRoboPlay->RP_DebugHex16 = &FT_DebugHex16;
#endif
}

//
// Check for ESC key pressed
//
boolean FT_EscPressed()
{
    OutPort(PPI_REG_C, (InPort(PPI_REG_C) & 0xF0) | 0x07);

    return (InPort(PPI_REG_B) & 0x04) ? false : true;
}

//
// Check for Right key pressed
//
boolean FT_RightPressed()
{
    boolean result = false;

    OutPort(PPI_REG_C, (InPort(PPI_REG_C) & 0xF0) | 0x08);

    if(!(InPort(PPI_REG_B) & 0x80))
    {
        result = true;
        while (!(InPort(PPI_REG_B) & 0x80));
    }

    return result;
}

//
// Check for Left key pressed
//
boolean FT_LeftPressed()
{
    boolean result = false;
    
    OutPort(PPI_REG_C, (InPort(PPI_REG_C) & 0xF0) | 0x08);

    if(!(InPort(PPI_REG_B) & 0x10))
    {
        result = true;
        while (!(InPort(PPI_REG_B) & 0x10));
    }

    return result;
}

//
// Main
//
void main(char *argv[], uint16_t argc) 
{
    uint8_t songArgument;
    REGS* regs;
    char* buffer;
    char  playerName[13];
    char* environment = "ROBOPLAY";

    uint8_t instrument = 0;

    printf("RoboPlay v0.6 - MSX Multi music format player for OPL3/4\n\r");
    printf("Copyright (C) 2020 RoboSoft Inc.\n\r");

    printf("\n\rDetecting OPL4 device ");
    if(!FT_OPL4_Detect())
    {
       printf("...Not Found\n\r");
       Exit(0);
    }
    else
    {
        printf("...Found\n\r");
    }

    FT_OPL4_Init();

    printf("Total OPL4 sample memory found: %iK\n\r", FT_OPL4_DetectRam() * 64);

    InitRamMapperInfo(4);
    FT_AllocateSegments();

    playerFileHandle = -1;

    if(argc == 0)
    {
        printf("\n\rUsage: RoboPlay [<player>.PLY] <song>.EXT\n\r");
        printf("Player name is optional. When no player is provided,\n\rthe songs file extension is used for identification\n\r");
        printf("\n\rUse environment item \"ROBOPLAY\" for players path\n\r");
        printf("E.g. SET ROBOPLAY = A:\\ROBOPLAY\\");
        Exit(0);
    }
    
    buffer = MMalloc(256 + 13);

    regs = _REGs();
    regs->hl = (unsigned int)environment;
    regs->de = (unsigned int)buffer;
    regs->bc = (unsigned int)0xff6b;
    IntDos();

    if(argc == 1)
    {
        sprintf(playerName, "%s.PLY", &argv[0][StrChr(argv[0],'.') + 1]);
        StrConcat(buffer, playerName);

        songArgument = 0;
    }
    else
    {
        StrConcat(buffer, argv[0]);
        songArgument = 1;
    }
    FT_LoadPlayer(buffer);

    if(!iRoboPlay->RP_Load(argv[songArgument]))
    {
        printf(" ...Error\n\r");
        printf("Not a valid file for this player\n\r");
        Exit(__INTER);
    }
    else
    {
        printf(" ...Done\n\r");
    }

    printf("\n\r");
    printf("Title       : %s\n\r", iRoboPlay->RP_GetTitle());
    printf("Author      : %s\n\r", iRoboPlay->RP_GetAuthor());
    printf("Description : %s\n\r", iRoboPlay->RP_GetDescription());

    if(iRoboPlay->RP_GetSubSongs() > 1)
    {
        printf("Number of subsongs: %d\n\r", iRoboPlay->RP_GetSubSongs());
        printf("\nNow playing ...ESC to stop, LEFT/RIGHT for subsong selection\n\r");
    }
    else
    {
        printf("\nNow playing ...ESC to stop\n\r");
    }

#ifndef __DEBUG
    DisableInterupt();
#endif

    iRoboPlay->RP_Rewind(0);
 #ifndef __DEBUG
    FT_SetRefresh();
#endif

    subsong = 0;
    while(!FT_EscPressed())
    {
        if(!iRoboPlay->RP_Update())
        {
            iRoboPlay->RP_Rewind(subsong);
        }

        if(iRoboPlay->RP_GetSubSongs() > 1)
        {
            if(FT_RightPressed() && subsong < iRoboPlay->RP_GetSubSongs())
            {
                subsong++;
                iRoboPlay->RP_Rewind(subsong);
            }

            if(FT_LeftPressed() && subsong > 0)
            {
                subsong--;
                iRoboPlay->RP_Rewind(subsong);
            }
        }

#ifndef __DEBUG
        while(!(FT_OPL4_Read() & 0x80));
        FT_OPL4_FM1_Write(4, 0x80);
#endif
#ifdef __DEBUG
        while(!FT_RightPressed());
#endif
    }   

    FT_OPL4_Init();
 
    FT_SetSegment(0);
    EnableInterupt();

    KillKeyBuffer();
    Exit(0);
}