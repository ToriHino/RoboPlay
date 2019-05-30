//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// RAD: Reality ADLib Tracker player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "rad.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

boolean RP_Load(char* fileName)
{  
    iRoboPlay = (void *)ROBO_PLAYER_BASE;

    filedata = (uint8_t*)SEGMENT_BASE;

    iRoboPlay->RP_Open(fileName);
    iRoboPlay->RP_Read(filedata, SEGMENT_SIZE);
    iRoboPlay->RP_Close();

    header = (RAD_HEADER*)filedata;

    if(strncmp(header->id,"RAD by REALiTY!!",16) || header->version != 0x10) return false;

    filedata = (uint8_t *)SEGMENT_BASE + sizeof(RAD_HEADER);
    if(header->flags & 128)
    {
        while(*filedata++);
    }

    speed = header->flags & 0x1F;
    hertz = 50;
    if(header->flags & 0x40) hertz = 18.0;

    while(*filedata)
    {
        instruments[*filedata - 1] = (RAD_INST *)++filedata;
        filedata += sizeof(RAD_INST);
    }

    orderLength = *++filedata;
    orderList = ++filedata;
    filedata += orderLength;

    patterns = filedata;

    return true;
}

boolean RP_Update()
{
    uint8_t i;

    FT_PlayLine();

//    for(i = 0; i < RAD_MAX_CHANNEL; i++) FT_ContinueFX(i);

    return true;
}

void RP_Rewind(int8_t subsong)
{
    uint8_t i;

    subsong;

    speedCnt = 1;
    order = 0;
    pattern = FT_GetPattern();
    line = 0;
    masterVol = 64;

    for(i = 0; i < RAD_MAX_CHANNEL; i++)
    {
        channels[i].instrument = 0;
        channels[i].volume = 0;
        channels[i].keyFlags = 0;
        channels[i].currFreq = 0;
        channels[i].currOctave = 0;
        channels[i].fx.portSlide = 0;
        channels[i].fx.volSlide = 0;
        channels[i].fx.toneSlideFreq = 0;
        channels[i].fx.toneSlideOct = 0;
        channels[i].fx.toneSlideSpeed = 0;
        channels[i].fx.toneSlideDir = 0;
    }

    iRoboPlay->RP_WriteOpl1(1, 0x20);
    iRoboPlay->RP_WriteOpl1(8, 0x0);
    iRoboPlay->RP_WriteOpl1(0xBD, 0);
    iRoboPlay->RP_WriteOpl2(4, 0);
    iRoboPlay->RP_WriteOpl2(5, 0);
}

float RP_GetRefresh()
{
    return hertz;
}

uint8_t RP_GetSubSongs()
{
    return 0;
}

char* RP_GetPlayerInfo()
{
    return "Reality ADlib Tracker Player by RoboSoft Inc.";
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

uint8_t* FT_GetPattern()
{   
    uint8_t patternNumber = 0;
    uint16_t* patternList = (uint16_t *)patterns;

    if(order >= orderLength) order = 0;
    patternNumber = orderList[order];

    if(patternNumber & 0x80)
    {
        order = patternNumber & 0x7F;
        patternNumber = orderList[order] & 0x7F;
    }

    return (uint8_t *)(patternList[patternNumber] + SEGMENT_BASE);
}

void FT_PlayLine()
{
    uint8_t i;
    uint8_t lineId = 0;
    uint8_t channum = 0;
    boolean last = false;

    speedCnt--;
    if(speedCnt > 0) return;

    speedCnt = speed;
//    for(i = 0; i < RAD_MAX_CHANNEL; i++) FT_ResetFX(i);

    lineJump = -1;

    lineId = *pattern++;
    do
    {
        channum = *pattern & 15;
        offset = op_table[channum];

        last = FT_UnpackNote();
        if(channum == 0) FT_PlayNote(channum);
    } while (!last);

    line++;
    if(line > RAD_MAX_LINE || lineJump >= 0)
    {
        line = (lineJump >= 0) ? lineJump : 0;

        order++;
        pattern = FT_GetPattern();
    }
}

void FT_PlayNote(uint8_t channel)
{
    int8_t    val;
    uint8_t   slideSpeed;
/*
    if(effectNum == RAD_CMD_TONE_SLIDE)
    {
        if(noteNum > 0 && noteNum <= 12)
        {
            channels[channel].fx.toneSlideOct = octaveNum;
            channels[channel].fx.toneSlideFreq = NoteFreq[noteNum - 1];
        }
        goto toneslide;
    }
*/
    if(instNum > 0)
    {
        channels[channel].instrument = instruments[instNum - 1];
        channels[channel].keyFlags |= fKeyOff | fKeyOn;

        iRoboPlay->RP_WriteOpl1(0x20 + offset, channels[channel].instrument->modulator_20);
        iRoboPlay->RP_WriteOpl1(0x23 + offset, channels[channel].instrument->carrier_23);
        iRoboPlay->RP_WriteOpl1(0x60 + offset, channels[channel].instrument->modulator_60);
        iRoboPlay->RP_WriteOpl1(0x63 + offset, channels[channel].instrument->carrier_63);
        iRoboPlay->RP_WriteOpl1(0x80 + offset, channels[channel].instrument->modulator_80);
        iRoboPlay->RP_WriteOpl1(0x83 + offset, channels[channel].instrument->carrier_83);
        iRoboPlay->RP_WriteOpl1(0xe0 + offset, channels[channel].instrument->modulator_e0);
        iRoboPlay->RP_WriteOpl1(0xe3 + offset, channels[channel].instrument->carrier_e3);
        iRoboPlay->RP_WriteOpl1(0xc0 + channel, channels[channel].instrument->both_c0);
    }

    if(noteNum > 0)
    {
        if(noteNum == 15) channels[channel].keyFlags |= fKeyOff;
        FT_PlayNoteOPL(channel);
    }
/*
    switch(effectNum)
    {
        case RAD_CMD_SET_VOL:
            FT_SetVolume(channel, param);
            break;
        case RAD_CMD_SET_SPEED:
            speed = param;
            break;
        case RAD_CMD_PORTAMENTO_UP:
            channels[channel].fx.portSlide = param;
            break;
        case RAD_CMD_PORTAMENTO_DOWN:
            channels[channel].fx.portSlide = -(int8_t)param;
            break;
        case RAD_CMD_TONE_VOL_SLIDE:
        case RAD_CMD_VOL_SLIDE:
            {
                val = param;
                if(val >= 50) val = -(val - 50);
                channels[channel].fx.volSlide = val;
                if(effectNum != RAD_CMD_TONE_VOL_SLIDE) break;
            }
        case RAD_CMD_TONE_SLIDE:
toneslide:
                slideSpeed = param;
                if(slideSpeed) channels[channel].fx.toneSlideSpeed = slideSpeed;
                FT_GetSlideDir(channel);
            break;
        case RAD_CMD_JUMP_TO_LINE:
            if(param >= RAD_MAX_LINE) break;
            lineJump = param;
            break;
    } */
}

void FT_PlayNoteOPL(uint8_t channel)
{
    if(channels[channel].keyFlags | fKeyOff)
    {
        channels[channel].keyFlags &= ~(fKeyOff | fKeyedOn);
        iRoboPlay->RP_WriteOpl1(0xB0 + offset, 0);
    }

    if(noteNum == 15) return;

    channels[channel].currFreq = NoteFreq[noteNum - 1];
    channels[channel].currOctave = octaveNum;

    // Frequency low byte
    iRoboPlay->RP_WriteOpl1(0xA0 + offset, channels[channel].currFreq & 0xFF);

    // Frequency high bits + octave + key on
    if(channels[channel].keyFlags & fKeyOn)
        channels[channel].keyFlags = (channels[channel].keyFlags & ~fKeyOn) | fKeyedOn;

    iRoboPlay->RP_WriteOpl1(
            0xB0 + offset, 
            ((channels[channel].currFreq & 768) >> 8) +
            (octaveNum << 2) | 
            ((channels[channel].keyFlags & fKeyedOn) ? 0x20 : 0));
}

boolean FT_UnpackNote()
{
    uint8_t n, i;
    uint8_t note = 0;
    uint8_t chanid = *pattern++;

    instNum = 0;
    effectNum = 0;
    param = 0;

    n = *pattern++;
    note = n & 0x7F;

    instNum = (n & 0x80) >> 3;
    i = *pattern++;
    instNum += (i >> 4);
    effectNum = i & 0x0F;
    if(effectNum) param = *pattern++;

    noteNum = note & 0x0F;
    octaveNum = note >> 4;

    return ((chanid & 0x80) != 0);
}

void FT_SetVolume(uint8_t channel, uint8_t volume)
{
    volume = (volume <= 63) ? volume : 63;
    channels[channel].volume = volume;

    iRoboPlay->RP_WriteOpl1(0x40 + op_table[channel], 63 - volume + (channels[channel].instrument->modulator_40 & 192));
    iRoboPlay->RP_WriteOpl1(0x43 + op_table[channel], 63 - volume + (channels[channel].instrument->carrier_43 & 192));
}

void FT_ResetFX(uint8_t channel)
{
    channels[channel].fx.portSlide = 0;
    channels[channel].fx.volSlide = 0;
    channels[channel].fx.toneSlideDir = 0;
}

void FT_ContinueFX(uint8_t channel)
{
    int8_t vol = 0;

    if(channels[channel].fx.portSlide)
        FT_Portamento(channel, channels[channel].fx.portSlide, false);

    if(channels[channel].fx.volSlide)
    {
        vol = channels[channel].volume;
        vol -= channels[channel].fx.volSlide;
        if(vol < 0) vol = 0;
        FT_SetVolume(channel, vol);
    }

    if(channels[channel].fx.toneSlideDir)
        FT_Portamento(channel, channels[channel].fx.toneSlideDir, true);
}

void FT_Portamento(uint8_t channel, int8_t amount, boolean toneSlide)
{
    uint8_t oct = channels[channel].currOctave;
    uint16_t freq = channels[channel].currFreq;

    freq += amount;

    if(freq < 0x156)
    {
        if(oct > 0)
        {
            oct --;
            freq += 0x2AE - 0x156;
        } else freq = 0x156;
    } else if(freq > 0x2AE)
    {
        if(oct < 7)
        {
            oct ++;
            freq -= 0x2AE - 0x156;
        } else freq = 0x2AE;
    }

    if(toneSlide)
    {
        if(amount >= 0)
        {
            if(oct > channels[channel].fx.toneSlideOct || 
              (oct == channels[channel].fx.toneSlideOct && freq >= channels[channel].fx.toneSlideFreq))
            {
                freq = channels[channel].fx.toneSlideFreq;
                oct = channels[channel].fx.toneSlideOct;
            } 
        } else
        {
            if(oct < channels[channel].fx.toneSlideOct || 
              (oct == channels[channel].fx.toneSlideOct && freq <= channels[channel].fx.toneSlideFreq))
            {
                freq = channels[channel].fx.toneSlideFreq;
                oct = channels[channel].fx.toneSlideOct;
            } 
        }
    }

    channels[channel].currFreq = freq;
    channels[channel].currOctave = oct;

    iRoboPlay->RP_WriteOpl1(0xA + offset, freq & 0xFF);
    iRoboPlay->RP_WriteOpl1(
                0xB0 + offset, 
                ((freq & 768) >> 8) +
                (oct << 2) | 
                ((channels[channel].keyFlags & fKeyOn) ? 0x20 : 0));
}

void FT_GetSlideDir(uint8_t channel)
{
    int8_t speed = channels[channel].fx.toneSlideSpeed;

    uint8_t oct;
    uint16_t freq;

    uint8_t oldoct;
    uint16_t oldfreq;

    if(speed > 0)
    {
        oct = channels[channel].fx.toneSlideOct;
        freq = channels[channel].fx.toneSlideFreq;

        oldoct = channels[channel].currOctave;
        oldfreq = channels[channel].currFreq;

        if(oldoct > oct) speed = -speed;
        else if(oldoct == oct)
        {
            if(oldfreq > freq) speed = -speed;
            else if (oldfreq == freq) speed = 0;
        }
    }

    channels[channel].fx.toneSlideDir = speed;
}