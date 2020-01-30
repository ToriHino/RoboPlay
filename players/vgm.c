//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// VGM: Video Game Music player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "vgm.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

boolean RP_Load(char* fileName)
{  
    uint8_t  i, currentSegment = 0;
    uint16_t dataSize;
    uint16_t readSize = 0;
    uint16_t pageLeft  = SEGMENT_SIZE;
    uint16_t bytesRead = 0;

    uint8_t* destination = (uint8_t *)SEGMENT_BASE;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;
    iRoboPlay->RP_Open(fileName);

    iRoboPlay->RP_Read(&vgmHeader, sizeof(VGM_HEADER));

    if (strncmp(&vgmHeader.vgmIdent, VGM_HEADER_ID, 4))
    {
        iRoboPlay->RP_Close();        
        return false;
    }

    if(vgmHeader.version < 0x151)
    {
        iRoboPlay->RP_Close();        
        return false;
    }

    deviceType = VGM_DEVICE_ID_YM3526;
    clock = vgmHeader.ym3526Clock;
    if(!clock)
    {
        deviceType = VGM_DEVICE_ID_Y8950;
        clock = vgmHeader.y8950Clock;
    }

    if(!clock)
    {
        deviceType = VGM_DEVICE_ID_YM3812;
        clock = vgmHeader.ym3812Clock;
        if(clock & VGM_DUAL_BIT) deviceType = VGM_DEVICE_ID_YM3812_DUAL;
    }

    if(!clock)
    {
        deviceType = VGM_DEVICE_ID_YMF262;
        clock = vgmHeader.ymf262Clock;
    }

    if(!clock)
    {
        deviceType = VGM_DEVICE_ID_YMF278B;
        clock = vgmHeader.ymf278bClock;
    }

    if(!clock)
    {
        iRoboPlay->RP_Close();
        return false;
    }

    iRoboPlay->RP_Open(fileName);

    dataSize = OFFSET_DATA + vgmHeader.vgmDataOffset;
    while(dataSize > 0)
    {
        readSize = (dataSize < READ_BUFFER_SIZE) ? dataSize : READ_BUFFER_SIZE;
        bytesRead = iRoboPlay->RP_Read((void*)READ_BUFFER, readSize);
        dataSize -= bytesRead;
    }

    vgmSize = (vgmHeader.gd3Offset > 0) ? vgmHeader.gd3Offset - vgmHeader.vgmDataOffset : vgmHeader.eofOffset - vgmHeader.vgmDataOffset;

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
    uint8_t cmd, port, reg, val;

    if(delayCounter >= VGM_STEP_VALUE)
    {
        delayCounter -= VGM_STEP_VALUE;
    }

    while(delayCounter < VGM_STEP_VALUE)
    {
        if(indexPtr >= vgmSize)
        {
            return false;
        }

        cmd = GetVgmData();
        switch(cmd)
        {
            case CMD_OPL:
            case CMD_OPL1:
            case CMD_OPL2:
            case CMD_OPL3_PORT0:
                reg = GetVgmData();
                val = GetVgmData();
                if(reg != 2 && reg != 3 && reg != 4)
                    iRoboPlay->RP_WriteOpl1(reg, val);
                break;
            case CMD_OPL2_2ND:
            case CMD_OPL3_PORT1:
                reg = GetVgmData();
                val = GetVgmData();
                if(reg != 2 && reg != 3 && reg != 4)
                    iRoboPlay->RP_WriteOpl2(reg, val);
                break;
            case CMD_OPL4:
                port = GetVgmData();
                reg = GetVgmData();
                val = GetVgmData();
                if(reg != 2 && reg != 3 && reg != 4)
                {
                    (port == 0) ? iRoboPlay->RP_WriteOpl1(reg, val) : iRoboPlay->RP_WriteOpl2(reg, val);
                }
                break;
            case CMD_WAIT:
                delayCounter = GetVgmData();
                delayCounter |= GetVgmData() << 8;
                break;
            case CMD_WAIT_735:
                delayCounter = 735;
                break;
            case CMD_WAIT_882:
                delayCounter = 882;
                break;
            case CMD_DATA_END:
                indexPtr = vgmSize;
                break;
            default:
                if(cmd >= CMD_WAIT_N && cmd <= CMD_WAIT_N + 0xF)
                {
                    delayCounter = (cmd & 0x0F) + 1;
                }
        }

        if(indexPtr >= vgmSize && vgmHeader.loopOffset) 
        {
            indexPtr = vgmHeader.loopOffset;

            songdata = (void*)SEGMENT_BASE;
            songdata += (indexPtr / SEGMENT_SIZE);
            iRoboPlay->RP_SetSegment(segmentList[indexPtr % SEGMENT_SIZE]);
        }
    }

    return true;
}

void RP_Rewind(int8_t subsong)
{
    // No subsongs in this format
    subsong;

    // Start with standard OPL2 mode
    iRoboPlay->RP_WriteOpl2(0x05, 0);

    currentSegment = 0;
    iRoboPlay->RP_SetSegment(segmentList[currentSegment]);
    songdata = (void*)SEGMENT_BASE;

    delayCounter = 0;
    indexPtr = 0;
}

float RP_GetRefresh()
{
    return VGM_REPLAY_FREQ;
}

uint8_t RP_GetSubSongs()
{
    return 0;
}

char* RP_GetPlayerInfo()
{
    return "Video Game Music (VGM) Player by RoboSoft Inc.";
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
    uint8_t i;

    for(i = 0; i < VGM_DEVICE_ID_CARD; i++)
        if(deviceDescriptions[i].id == deviceType) break;
    
    return deviceDescriptions[i].description;
}

uint8_t GetVgmData()
{
    uint8_t data = *songdata++;
    if(songdata == (void*)(SEGMENT_BASE + SEGMENT_SIZE))
    {
        songdata = (void*)SEGMENT_BASE;
        iRoboPlay->RP_SetSegment(segmentList[++currentSegment]);
    }

    indexPtr++;

    return data;
}