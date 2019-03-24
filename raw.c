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
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/newTypes.h"

#include "player.h"
#include "raw.h"

ROBO_PLAYER_HEADER* header_ptr;

boolean FT_load(char* fileName)
{  
    uint8_t  i;
    uint16_t bytesRead;
    uint8_t* destination;

    header_ptr = (void *)ROBO_PLAYER_BASE;
    header_ptr->FT_Open_ptr(fileName);

    header_ptr->FT_Read_ptr(&rawHeader, sizeof(RAW_HEADER));
    if (strncmp(rawHeader.signature, "RAWADATA", sizeof(rawHeader.signature)))
    {
        header_ptr->FT_Close_ptr();        
        return false;
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

        if(pos >= PAGE_SIZE/2)
        {
            pos = 0;
            header_ptr->FT_SetSegment_ptr(++currentSegment);
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
                    if(pos >= PAGE_SIZE/2)
                    {
                        pos = 0;
                        header_ptr->FT_SetSegment_ptr(++currentSegment);
                    }
                    tmp = songdata[pos].command << 8;
                    speed = songdata[pos].param + tmp;
                    header_ptr->FT_UpdateRefresh_ptr();
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
                        header_ptr->FT_WriteOpl2_ptr(songdata[pos].command, songdata[pos].param);
                    else
                        header_ptr->FT_WriteOpl1_ptr(songdata[pos].command, songdata[pos].param);
                }
        }
    } while (songdata[pos++].command || setspeed);

    return true;
}

void FT_rewind(int8_t subsong)
{
    // No subsongs in this format
    subsong;

    currentSegment = 0;
    header_ptr->FT_SetSegment_ptr(segmentList[currentSegment]);
    songdata = (RAW_DATA*)TRACK_DATA;
    pos = 0;

    delayCounter = 0;
    speed = rawHeader.clock;
    highOPL = false;

    header_ptr->FT_WriteOpl1_ptr(1, 32);    // Go to 9 channel mode
}

float FT_getRefresh()
{ 
    return 1193180.0 / (float)(speed ? speed : 0xffff);
}

uint8_t FT_getSubSongs()
{
    return 0;
}

char* FT_getPlayerInfo()
{
    return "Raw AdLib Capture player by RoboSoft Inc.";
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
