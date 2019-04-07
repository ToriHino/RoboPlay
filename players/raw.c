//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// RAW: RAW OPL capture player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "raw.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

boolean RP_Load(char* fileName)
{  
    uint8_t  i;
    uint16_t bytesRead;
    uint8_t* destination;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;
    iRoboPlay->RP_Open(fileName);

    iRoboPlay->RP_Read(&rawHeader, sizeof(RAW_HEADER));
    if (strncmp(rawHeader.signature, "RAWADATA", sizeof(rawHeader.signature)))
    {
        iRoboPlay->RP_Close();        
        return false;
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

    for(i = 0; i < 2; i++)
    {
        if(destination >= (uint8_t*)SEGMENT_SIZE)
        {
            segmentList[currentSegment] = iRoboPlay->RP_AllocateSegment();
            iRoboPlay->RP_SetSegment(segmentList[currentSegment++]);
            destination = (uint8_t*)SEGMENT_BASE;
        }
        *destination++ = 0xff;
    }

    iRoboPlay->RP_Close();

    return true;
}

boolean RP_Update()
{
    uint16_t tmp;

    boolean setspeed = false;

    if(delayCounter)
    {
        delayCounter--;
        return true;
    }

    do
    {
        setspeed = false;

        if(pos >= SEGMENT_SIZE/2)
        {
            pos = 0;
            iRoboPlay->RP_SetSegment(++currentSegment);
        }

        switch(songdata[pos].command)
        {
            case RAW_DELAY:
                delayCounter = songdata[pos].param - 1;
                break;

            case RAW_CONTROL:
                if(songdata[pos].param == RAW_CONTROL_TYPE_CLOCK)
                {
                    setspeed = true;
                    pos++;
                    if(pos >= SEGMENT_SIZE/2)
                    {
                        pos = 0;
                        iRoboPlay->RP_SetSegment(++currentSegment);
                    }
                    tmp = songdata[pos].command << 8;
                    speed = songdata[pos].param + tmp;
                    iRoboPlay->RP_UpdateRefresh();
                }
                else
                {
                    highOPL = (songdata[pos].param == RAW_CONTROL_TYPE_HIGH_OPL);
                }
                break;

            case 0xff:
                if(songdata[pos].param == 0xff) return false;
                break;

            default:
                if(songdata[pos].command != 2 && songdata[pos].command != 3 && songdata[pos].command != 4)
                {
                    if(highOPL)
                        iRoboPlay->RP_WriteOpl2(songdata[pos].command, songdata[pos].param);
                    else
                        iRoboPlay->RP_WriteOpl1(songdata[pos].command, songdata[pos].param);
                }
        }
    } while (songdata[pos++].command || setspeed);

    return true;
}

void RP_Rewind(int8_t subsong)
{
    // No subsongs in this format
    subsong;

    currentSegment = 0;
    iRoboPlay->RP_SetSegment(segmentList[currentSegment]);
    songdata = (RAW_DATA*)SEGMENT_BASE;
    pos = 0;

    delayCounter = 0;
    speed = rawHeader.clock;
    highOPL = false;

    iRoboPlay->RP_WriteOpl1(1, 32);    // Go to 9 channel mode
}

float RP_GetRefresh()
{ 
    return 1193180.0 / (float)(speed ? speed : 0xffff);
}

uint8_t RP_GetSubSongs()
{
    return 0;
}

char* RP_GetPlayerInfo()
{
    return "Raw AdLib Capture player by RoboSoft Inc.";
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
