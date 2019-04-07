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
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "dro2.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

boolean RP_Load(char* fileName)
{  
    uint8_t  i;
    uint16_t bytesRead;
    uint8_t* destination;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;
    iRoboPlay->RP_Open(fileName);

    iRoboPlay->RP_Read(&dro2Header, sizeof(DRO2_HEADER));
    if (strncmp(dro2Header.signature, "DBRAWOPL", sizeof(dro2Header.signature)))
    {
        iRoboPlay->RP_Close();        
        return false;
    }
    if(dro2Header.versionMajor != 2)
    {
        iRoboPlay->RP_Close();        
        return false;
    }
    iRoboPlay->RP_Read(codeMap, dro2Header.codeMapLength);

    if(dro2Header.hardwareType == HARDWARETYPE_OPL3 || dro2Header.hardwareType == HARDWARETYPE_DUAL_OPL2)
    {
        iRoboPlay->RP_WriteOpl2(0x5, 0x3);
    }

    currentSegment = 0;
    do {
        segmentList[currentSegment] = iRoboPlay->RP_AllocateSegment();
        iRoboPlay->RP_SetSegment(segmentList[currentSegment++]);

        destination = (uint8_t*)SEGMENT_BASE;
        for(i = 0; i < SEGMENT_SIZE / READ_BUFFER_SIZE; i++)
        {
            // It's not possible to read directly to non-primary mapper memory segments,
            // so use a buffer inbetween.
            bytesRead = iRoboPlay->RP_Read((void*)READ_BUFFER, READ_BUFFER_SIZE);
            if(!bytesRead) break;

            memcpy(destination, (void*)READ_BUFFER, READ_BUFFER_SIZE);
            destination += READ_BUFFER_SIZE;
        }
    } while (bytesRead);

    iRoboPlay->RP_Close();

    return true;
}

boolean RP_Update()
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
        if(songdata == (void*)(SEGMENT_BASE + SEGMENT_SIZE))
        {
            songdata = (void*)SEGMENT_BASE;
            iRoboPlay->RP_SetSegment(segmentList[++currentSegment]);
        }
        value = *songdata++;
        if(songdata == (void*)(SEGMENT_BASE + SEGMENT_SIZE))
        {
            songdata = (void*)SEGMENT_BASE;
            iRoboPlay->RP_SetSegment(segmentList[++currentSegment]);
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
                    iRoboPlay->RP_WriteOpl2(actualRegister, value);
            }
            else
            {
                actualRegister = codeMap[registerIndex];
                if(actualRegister != 2 && actualRegister != 3 && actualRegister != 4)
                    iRoboPlay->RP_WriteOpl1(actualRegister, value);
            }
        }
    }
    return true;
}

void RP_Rewind(int8_t subsong)
{
    // No subsongs in this format
    subsong;

    currentSegment = 0;
    iRoboPlay->RP_SetSegment(segmentList[currentSegment]);
    songdata = (void*)SEGMENT_BASE;

    delayCounter = 0;
    indexPtr = 0;
}

float RP_GetRefresh()
{
    // Fixed replay rate of 1000Hz
    return 1000.0;
}

uint8_t RP_GetSubSongs()
{
    return 0;
}

char* RP_GetPlayerInfo()
{
    return "DOSBox Raw OPL type 2.0 Player by RoboSoft Inc.";
}

char* RP_GetTitle()
{
    return "-";
}

char* RP_GetAuthor()
{
    return "-";
}

char* RP_GetDescription()
{
    return "-";
}