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

ROBO_PLAYER_HEADER* header_ptr;
uint16_t playerFileHandle;

SEGMENT_INFO segmentList[MAX_NR_SEGMENTS];
uint8_t segmentsFound;
uint8_t segmentAllocationIndex;
uint8_t currentSlot;

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
        ld hl,#TRACK_DATA
        call ENASLT
        __endasm;
    }
}

//
// Output to OPL port 1
// 

void FT_WriteOpl1(uint8_t addr, uint8_t value)
{
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
}

//
// Output to OPL port 2
// 

void FT_WriteOpl2(uint8_t addr, uint8_t value)
{
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
    OutPort(OPL4_DATA, value);
}

//
// Set refresh rate
//
void FT_SetRefresh(float refresh)
{
    if( refresh > 50.0)
    {
        FT_WriteOpl1(OPL4_TIMER1_COUNT, 255 - (uint8_t)( (1000.0 / refresh) / 0.08));
        FT_WriteOpl1(4, 0x01);
    }
    else
    {       
        FT_WriteOpl1(OPL4_TIMER2_COUNT, 255 - (uint8_t)( (1000.0 / refresh) / 0.320) );
        FT_WriteOpl1(4, 0x02);
    }
}

//
// Reset OPL
//
void FT_ResetOPL()
{
    uint8_t i = 0;

    FT_WriteOpl2(5, 3);
    FT_WriteOpl1(1, 0);
    FT_WriteOpl1(2, 0);
    FT_WriteOpl1(3, 0);
    FT_WriteOpl1(4, 0);
    FT_WriteOpl1(8, 0);

    FT_WriteOpl2(4, 0);
    FT_WriteOpl1(4, 0x80);

    for(i = 20; i < 0xf6; i++)
    {
        FT_WriteOpl1(i, 0);
        FT_WriteOpl2(i, 0);
    }

    FT_WriteOpl2(5, 0);
}

void FT_Open(char *name)
{
    printf("Loading file: %s", name);

    playerFileHandle = Open(name, O_RDONLY);

    if(playerFileHandle == -1)
    {
        Exit(__NOFIL);
    }
}

uint16_t FT_Read(void *buf, uint16_t nbytes)
{
    return Read(playerFileHandle, buf, nbytes);
}

void FT_Close()
{
    Close(playerFileHandle);
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

    header_ptr = (void *)ROBO_PLAYER_BASE;
    Read(fH, header_ptr, 0x4000);
    if(strncmp(header_ptr->signature,"ROBOPLAY", 8) != 0)
    {
        printf(" ...Error\n\r");
        printf("\n\rNot a RoboPlay player file\n\r");
        Close(fH);
        Exit(__INTER);
    }
    Close(fH);

    printf(" ...Done\n\r");

    printf("%s\n\n\r", header_ptr->FT_getPlayerInfo_ptr());

    header_ptr->FT_Open_ptr =  &FT_Open;
    header_ptr->FT_Read_ptr =  &FT_Read;
    header_ptr->FT_Close_ptr = &FT_Close;
    
    header_ptr->FT_AllocateSegment_ptr = &FT_AllocateSegment;
    header_ptr->FT_SetSegment_ptr = &FT_SetSegment;

    header_ptr->FT_SetRefresh_ptr = &FT_SetRefresh;

    header_ptr->FT_WriteOpl1_ptr = &FT_WriteOpl1;
    header_ptr->FT_WriteOpl2_ptr = &FT_WriteOpl2;
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
// Main
//
void main(char *argv[], uint16_t argc) 
{
    float refresh;
    argc;

    printf("RoboPlay v0.3 - Multi format player for OPL3/4\n\r");
    printf("Copyright (C) 2019 RoboSoft Inc.\n\r");

    InitRamMapperInfo(4);
    FT_AllocateSegments();

    FT_LoadPlayer(argv[0]);
    if(!header_ptr->FT_load_ptr(argv[1]))
    {
        printf(" ...Error\n\r");
        printf("Not a valid file for this player\n\r");
        Exit(__INTER);
    }

    printf(" ...Done\n\r");

    printf("Title      : %s\n\r", header_ptr->FT_getTitle_ptr());
    printf("Author     : %s\n\r", header_ptr->FT_getAuthor_ptr());
    printf("Description: %s\n\r", header_ptr->FT_getDescription_ptr());

    if(header_ptr->FT_getSubSongs_ptr() > 1)
    {
        printf("Number of subsongs: %d\n\r", header_ptr->FT_getSubSongs_ptr());
    }

    printf("\nNow playing ...ESC to stop\n\r");

    DisableInterupt();

    FT_ResetOPL();

    header_ptr->FT_rewind_ptr(0);
    FT_SetRefresh(header_ptr->FT_getRefresh_ptr());

    while(!FT_EscPressed())
    {
        if(!header_ptr->FT_update_ptr())
        {
            header_ptr->FT_rewind_ptr(0);
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