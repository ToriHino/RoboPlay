//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// DRO2: DOSBox Raw OPL v2.0 player

#include <string.h>
#include <stdint.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/newTypes.h"

#include "player.h"
#include "dro2.h"

ROBO_PLAYER_HEADER* header_ptr;

boolean FT_load(char* fileName)
{  
    uint8_t  i;
    uint16_t bytesRead;
    uint8_t* destination;

    header_ptr = (void *)ROBO_PLAYER_BASE;
    header_ptr->FT_Open_ptr(fileName);

    header_ptr->FT_Read_ptr(&dro2Header, sizeof(DRO2_HEADER));
    if (strncmp(dro2Header.signature, "DBRAWOPL", sizeof(dro2Header.signature)))
    {
        header_ptr->FT_Close_ptr();        
        return false;
    }
    if(dro2Header.versionMajor != 2)
    {
        header_ptr->FT_Close_ptr();        
        return false;
    }
    header_ptr->FT_Read_ptr(codeMap, dro2Header.codeMapLength);

    if(dro2Header.hardwareType == HARDWARETYPE_OPL3 || dro2Header.hardwareType == HARDWARETYPE_DUAL_OPL2)
    {
        header_ptr->FT_WriteOpl2_ptr(0x5, 0x3);
    }

    currentSegment = 0;
    do {
        segmentList[currentSegment] = header_ptr->FT_AllocateSegment_ptr();
        header_ptr->FT_SetSegment_ptr(segmentList[currentSegment++]);

        destination = (uint8_t*)TRACK_DATA;
        for(i = 0; i < PAGE_SIZE / READ_BUFFER_SIZE; i++)
        {
            // It's not possible to read directly to non-primary mapper memory segments,
            // so use a buffer inbetween.
            bytesRead = header_ptr->FT_Read_ptr((void*)READ_BUFFER, READ_BUFFER_SIZE);
            if(!bytesRead) break;

            memcpy(destination, (void*)READ_BUFFER, READ_BUFFER_SIZE);
            destination += READ_BUFFER_SIZE;
        }
    } while (bytesRead);

    header_ptr->FT_Close_ptr();

    return true;
}

boolean FT_update()
{
    uint8_t registerIndex;
    uint8_t value;
    uint8_t actualRegister;

    if(delayCounter)
    {
        delayCounter--;
    }

    while(!delayCounter)
    {
        indexPtr++;
        if(indexPtr > dro2Header.lengthPairs)
        {
            return false;
        }

        registerIndex = *songdata++;
        if(songdata == (void*)(TRACK_DATA + PAGE_SIZE))
        {
            songdata = (void*)TRACK_DATA;
            header_ptr->FT_SetSegment_ptr(segmentList[++currentSegment]);
        }
        value = *songdata++;
        if(songdata == (void*)(TRACK_DATA + PAGE_SIZE))
        {
            songdata = (void*)TRACK_DATA;
            header_ptr->FT_SetSegment_ptr(segmentList[++currentSegment]);
        }

        if(registerIndex == dro2Header.shortDelayCode)
        {
            delayCounter = value + 1;
        }
        else if(registerIndex == dro2Header.longDelayCode)
        {
            delayCounter = (value + 1) << 8;
        }
        else
        {
            if(registerIndex & 0x80)
            {
                registerIndex &= 0x7f;
                actualRegister = codeMap[registerIndex];
                if(actualRegister != 2 && actualRegister != 3 && actualRegister != 4)
                    header_ptr->FT_WriteOpl2_ptr(actualRegister, value);
            }
            else
            {
                actualRegister = codeMap[registerIndex];
                if(actualRegister != 2 && actualRegister != 3 && actualRegister != 4)
                    header_ptr->FT_WriteOpl1_ptr(actualRegister, value);
            }
        }
    }
    return true;
}

void FT_rewind(byte subsong)
{
    // No subsongs in this format
    subsong;

    currentSegment = 0;
    header_ptr->FT_SetSegment_ptr(segmentList[currentSegment]);
    songdata = (void*)TRACK_DATA;

    delayCounter = 0;
    indexPtr = 0;
}

float FT_getRefresh()
{
    // Fixed replay rate of 1000Hz
    return 1000.0;
}

char* FT_getPlayerInfo()
{
    return "DOSBox Raw OPL type 2.0 Player by RoboSoft Inc.";
}

char* FT_getTitle()
{
    return "-";
}

char* FT_getAuthor()
{
    return "-";
}

char* FT_getDescription()
{
    return "-";
}