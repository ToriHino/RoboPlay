//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
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

ROBO_PLAYER_INTERFACE* iRoboPlay;
uint16_t playerFileHandle;

SEGMENT_INFO segmentList[MAX_NR_SEGMENTS];
uint8_t segmentsFound;
uint8_t segmentAllocationIndex;
uint8_t currentSlot;

uint8_t subsong;

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
    if(segmentAllocationIndex >= segmentsFound)
    {
        Exit(_NORAM);
    }

    return segmentAllocationIndex++;
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
// Output to OPL port 1
// 

void FT_WriteOpl1(uint8_t addr, uint8_t value)
{
    addr;
    value;

    __asm
    ld  hl, #2+0
    add hl, sp
    ld  a,(hl)
    out (OPL4_REG),a
    ld  hl, #3+0
    add hl, sp
    ld  a,(hl)
    out (OPL4_DATA),a
    __endasm;

/*
    uint8_t busy = 0;

    do
    {
        busy = InPort(OPL4_REG);
    } while(busy & 1);
    OutPort(OPL4_REG, addr);

    do 
    {
        busy = InPort(OPL4_REG);
    } while(busy & 1);
    OutPort(OPL4_DATA, value);   
*/
}

//
// Output to OPL port 2
// 

void FT_WriteOpl2(uint8_t addr, uint8_t value)
{
    addr;
    value;
    
    __asm
    ld  hl, #2+0
    add hl, sp
    ld  a,(hl)
    out (OPL4_REG2),a
    ld  hl, #3+0
    add hl, sp
    ld  a,(hl)
    out (OPL4_DATA2),a
    __endasm;
/*   
    uint8_t busy = 0;

    do    
    {
        busy = InPort(OPL4_REG);
    } while(busy & 1);

    OutPort(OPL4_REG2, addr);

    do 
    {
        busy = InPort(OPL4_REG);
    } while(busy & 1);
    OutPort(OPL4_DATA2, value);
*/
}

//
// Set refresh rate
//
void FT_SetRefresh()
{
    float refresh = iRoboPlay->RP_GetRefresh();
    if( refresh > 50.0)
    {
        FT_WriteOpl1(OPL4_TIMER1_COUNT, 255 - (uint8_t)( (1000.0 / refresh) / 0.08));
        FT_WriteOpl1(4, 0x21);
        FT_WriteOpl1(4, 0x80);
    }
    else
    {       
        FT_WriteOpl1(OPL4_TIMER2_COUNT, 255 - (uint8_t)( (1000.0 / refresh) / 0.320) );
        FT_WriteOpl1(4, 0x42);
        FT_WriteOpl1(4, 0x80);
    }
}

//
// Reset OPL
//
void FT_ResetOPL()
{
    uint8_t i = 0, val;

    FT_WriteOpl2(5, 3);
    
    FT_WriteOpl1(1, 0);
    FT_WriteOpl1(2, 0);
    FT_WriteOpl1(3, 0);
    FT_WriteOpl1(4, 0);
    FT_WriteOpl1(8, 0);

    FT_WriteOpl2(1, 0);
    FT_WriteOpl2(2, 0);
    FT_WriteOpl2(3, 0);
    FT_WriteOpl2(4, 0);
    FT_WriteOpl2(8, 0);

    for(i = 20; i < 0xf6; i++)
    {
        val = (i >= 0x60 && i < 0xA0) ? 0xFF : 0;
        FT_WriteOpl1(i, val);
        FT_WriteOpl2(i, val);
    }

    FT_WriteOpl1(4, 0x80);
    FT_WriteOpl2(5, 0);
}

//
// Open file
//
void FT_Open(char *name)
{
    printf("Loading file: %s", name);

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
// Close file
//
void FT_Close()
{
    Close(playerFileHandle);
    printf(" ...Done\n\r");
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

//
// Load player module
//
void FT_LoadPlayer(char* fileName)
{
    uint16_t fH = 0;
    uint8_t i = 0;
  
    printf("\nLoading player: %s", fileName);
    fH = Open(fileName, O_RDONLY);
    if(fH == -1)
    {
        Exit(__NOFIL);
    }

    iRoboPlay = (void *)ROBO_PLAYER_BASE;
    Read(fH, iRoboPlay, 0x4000);
    if(strncmp(iRoboPlay->signature,"ROBOPLAY", 8) != 0)
    {
        printf(" ...Error\n\r");
        printf("\n\rNot a RoboPlay player file\n\r");
        Close(fH);
        Exit(__INTER);
    }
    Close(fH);

    printf(" ...Done\n\r");

    printf("%s\n\n\r", iRoboPlay->RP_GetPlayerInfo());

    iRoboPlay->RP_Open =  &FT_Open;
    iRoboPlay->RP_Read =  &FT_Read;
    iRoboPlay->RP_Close = &FT_Close;
    iRoboPlay->RP_Exists = &FT_Exists;
    
    iRoboPlay->RP_AllocateSegment = &FT_AllocateSegment;
    iRoboPlay->RP_SetSegment = &FT_SetSegment;

    iRoboPlay->RP_UpdateRefresh = &FT_SetRefresh;

    iRoboPlay->RP_WriteOpl1 = &FT_WriteOpl1;
    iRoboPlay->RP_WriteOpl2 = &FT_WriteOpl2;
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

    printf("RoboPlay v0.4 - Multi format player for OPL3/4\n\r");
    printf("Copyright (C) 2019 RoboSoft Inc.\n\r");

    InitRamMapperInfo(4);
    FT_AllocateSegments();

    if(argc == 0)
    {
        printf("\n\rUsage: RoboPlay [<player>.PLY] <song>.EXT\n\r");
        printf("Player name is optional. When no player is provided,\n\rthe songs file extension is used for identification\n\r");
        printf("\n\rUse environment item \"ROBOPLAY\" for players path\n\r");
        printf("E.g. ROBOPLAY = A:\\ROBOPLAY\\");
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

    DisableInterupt();

    FT_ResetOPL();

    iRoboPlay->RP_Rewind(0);
    FT_SetRefresh();

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

        while(!InPort(OPL4_REG) & 0x3);
        FT_WriteOpl1(4, 0x80);
    }   

    FT_ResetOPL();
 
    FT_SetSegment(0);
    EnableInterupt();

    KillKeyBuffer();
    Exit(0);
}