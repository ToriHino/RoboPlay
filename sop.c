//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// SOP: Note OPL3 Music Sequencer player

#include <string.h>
#include <stdint.h>
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/newTypes.h"

#include "player.h"
#include "sop.h"

ROBO_PLAYER_HEADER* header_ptr;

boolean FT_load(char* fileName)
{
    FT_InitVolumeTable();

    header_ptr = (void *)ROBO_PLAYER_BASE;

    header_ptr->FT_Open_ptr(fileName);
    header_ptr->FT_Read_ptr(&sopHeader, sizeof(SOP_HEADER));
    if(strncmp(sopHeader.signature,"sopepos", 7) != 0)
    {
        header_ptr->FT_Close_ptr();
        return false;
    }

    if((sopHeader.majorVersion != '\0') || (sopHeader.minorVersion != '\1'))
    {
        header_ptr->FT_Close_ptr();
        return false;
    }

    if(!sopHeader.basicTempo) sopHeader.basicTempo = SOP_DEF_TEMPO;

    header_ptr->FT_Read_ptr(chanMode, sopHeader.nTracks);

    FT_LoadSOPInstruments();
    FT_LoadSOPTrackData();

    header_ptr->FT_Close_ptr(); 

    return true; 
}

boolean FT_update()
{
    uint8_t i;
    boolean songEnd = true;

    for(i = 0; i < sopHeader.nTracks + 1; i++)
    {
        if(tracks[i].dur)
        {
            songEnd = false;
            if(!--tracks[i].dur) FT_NoteOff(i);
        }

        if(tracks[i].curEvent >= tracks[i].numEvents) continue;
        songEnd = false;

        if(!tracks[i].counter)
        {
            if(tracks[i].curEvent == 0 && tracks[i].ticks)
            {
                tracks[i].ticks++;
            }
        }

        if(++tracks[i].counter >= tracks[i].ticks)
        {
            tracks[i].counter = 0;
            header_ptr->FT_SetSegment_ptr(tracks[i].currentSegment);
            while(tracks[i].curEvent < tracks[i].numEvents)
            {
                FT_ExecuteCommand(i);
                if(tracks[i].curEvent >= tracks[i].numEvents) break;
                else
                {
                    tracks[i].ticks = FT_GetTrackData(i);
                    tracks[i].ticks |= FT_GetTrackData(i) << 8;
                }
                if(tracks[i].ticks) break;
            }
        }
    }

    return !songEnd;
}

void FT_rewind(int8_t subsong)
{
    uint8_t i = 0;

    subsong;

    FT_SetTempo(sopHeader.basicTempo);

    for(i = 0; i < YMB_SIZE; i++)
    {
        ymbuf[i] = 0;
        ymbuf[i + YMB_SIZE] = 0;
    }

    for (i = 0; i < 20; i++) 
    {
        vPitchBend[i] = 100;
        voiceKeyOn[i] = 0;
        voiceNote[i] = MID_C;
        VoiceVolume[i] = 0;
        Ksl[i] = 0;
        Ksl2[i] = 0;
        Ksl2V[i] = 0;
        OP4[i] = 0;
        Stereo[i] = 0x30;
    }

    OP_MASK = 0;

    for(i = 0; i < sopHeader.nTracks+1; i++)
    {
        tracks[i].currentSegment = tracks[i].startSegment;
        tracks[i].currentData = tracks[i].startData;

        tracks[i].curEvent = 0;
        tracks[i].dur = 0;
        tracks[i].counter = 0;

        header_ptr->FT_SetSegment_ptr(tracks[i].currentSegment);
        tracks[i].ticks = FT_GetTrackData(i);
        tracks[i].ticks |= FT_GetTrackData(i) << 8;

        if(chanMode[i] == SOP_CHAN_4OP)
        {
            FT_Set4OPMode(i, 1);
        }
    }

    for(i = 0; i < SOP_MAX_TRACK; i++)
    {
        volume[i] = 0;
        lastvol[i] = 0;
    }
    masterVolume = SOP_MAX_VOL;

    interval = (sopHeader.basicTempo * sopHeader.tickBeat) / 60;

    header_ptr->FT_WriteOpl2_ptr(5, 3); /* YMF-262M Mode */
    header_ptr->FT_WriteOpl1_ptr(8, 0);
    header_ptr->FT_WriteOpl1_ptr(1, 0x20);

    FT_SetPercussionMode(sopHeader.percussive);
}

float FT_getRefresh()
{
    return interval;
}

uint8_t FT_getSubSongs()
{
    return 0;
}

char* FT_getPlayerInfo()
{
    return "Note OPL3 Music Sequencer player by RoboSoft Inc.";
}

char* FT_getTitle()
{
    return sopHeader.title;
}

char* FT_getAuthor()
{
    return "-";
}

char* FT_getDescription()
{
    return sopHeader.comment;
}

void FT_ExecuteCommand(uint8_t track)
{
    uint8_t i, value = 0;

    uint8_t event = FT_GetTrackData(track);

    switch(event)
    {
         case SOP_EVNT_SPEC:
            break;
         case SOP_EVNT_NOTE:
            value = FT_GetTrackData(track);
            tracks[track].dur = FT_GetTrackData(track) + (FT_GetTrackData(track) << 8);
            FT_NoteOn(track, value);
            break;
         case SOP_EVNT_VOL:
            value = FT_GetTrackData(track);          
            lastvol[track] = value;
            value = value * masterVolume / SOP_MAX_VOL;
            if(value != volume[track])
            {
                FT_SetVoiceVolume(track, value);
                volume[track] = value;
            }
            break;
         case SOP_EVNT_PITCH:
            value = FT_GetTrackData(track);
            FT_SetVoicePitch(track, value);
            break;
         case SOP_EVNT_INST:
            value = FT_GetTrackData(track);
            FT_SetVoiceTimbre(track, instruments[value].instData);
            break;
         case SOP_EVNT_PAN:
            value = FT_GetTrackData(track);
            FT_SetStereoPAN(track, value);
            break;
         case SOP_EVNT_TEMPO:
            value = FT_GetTrackData(track);
            FT_SetTempo(value);
            break;
         case SOP_EVNT_MVOL:
            value = FT_GetTrackData(track);
            masterVolume = value;
            for(i = 0; i < sopHeader.nTracks; i++)
            {
                value = lastvol[i] * masterVolume / SOP_MAX_VOL;
                if(value != volume[i])
                {
                    FT_SetVoiceVolume(i, value);
                    volume[i] = value;
                }
            }
            break;
    }

    tracks[track].curEvent++;
}

void FT_InitVolumeTable()
{
    uint8_t i,j;
    
    VolumeTable = (uint8_t *)VOLUME_TABLE_BASE;
    for(i = 0; i < 64; i++)
    {
        for(j = 0; j < 128; j++)
        {
            VolumeTable[i * 128 + j] = (i * j + (MAX_VOLUME +1) /2) >> LOG2_VOLUME;
        }
    }
}

void FT_SetTempo(uint8_t tempo)
{
    interval = (tempo * sopHeader.tickBeat) / 60;

    header_ptr->FT_UpdateRefresh_ptr();
}

boolean FT_LoadSOPInstruments()
{
    uint8_t i;

    header_ptr->FT_SetSegment_ptr(0);

    instruments = (SOP_INST*)INSTRUMENTS_BASE;
    for(i = 0; i < sopHeader.nInsts; i++)
    {
        header_ptr->FT_Read_ptr(&instruments[i], 1+8+19);  // instType + shortName + longName
        switch(instruments[i].instType)
        {
            case INSTRUMENT_TYPE_UNUSED:
                break;
            case INSTRUMENT_TYPE_MELODY_4OP:
                header_ptr->FT_Read_ptr(&instruments[i].instData, INSTRUMENT_DATA_SIZE_4OP);
                break;
            default:
                header_ptr->FT_Read_ptr(&instruments[i].instData, INSTRUMENT_DATA_SIZE_2OP);
        }
    }

    return true;
}

boolean FT_LoadSOPTrackData()
{
    uint8_t i;

    uint32_t dataSize = 0;
    uint16_t readSize = 0;
    uint16_t pageLeft  = PAGE_SIZE;
    uint16_t bytesRead = 0;

    uint8_t* destination = (uint8_t *)SOP_EVNT_BASE;
    uint8_t  currentSegment = 1;

    header_ptr->FT_SetSegment_ptr(currentSegment);
    for(i = 0; i < sopHeader.nTracks+1; i++)
    {
        tracks[i].startSegment = currentSegment;
        tracks[i].startData = destination;

        header_ptr->FT_Read_ptr(&tracks[i].numEvents, sizeof(tracks[i].numEvents));
        header_ptr->FT_Read_ptr(&dataSize, sizeof(dataSize));

        while(dataSize > 0)
        {
            readSize = (dataSize < READ_BUFFER_SIZE) ? dataSize : READ_BUFFER_SIZE;
            readSize = (readSize < pageLeft) ? readSize : pageLeft;

            // It's not possible to read directly to non-primary mapper memory segments,
            // so use a buffer inbetween.
            bytesRead = header_ptr->FT_Read_ptr((void*)READ_BUFFER, readSize);
            memcpy(destination, (void*)READ_BUFFER, bytesRead);

            dataSize -= bytesRead;
            destination += bytesRead;
            pageLeft -= bytesRead;
            if(pageLeft == 0)
            {
                header_ptr->FT_SetSegment_ptr(++currentSegment);

                pageLeft = PAGE_SIZE;
                destination = (uint8_t *)SOP_EVNT_BASE;
            }
        }
    }

    return true;
}

void FT_WriteOpl1(uint8_t addr, uint8_t value)
{
    if(addr >= 0xB0) ymbuf[addr - 0xB0] = value;
    header_ptr->FT_WriteOpl1_ptr(addr, value);
}

void FT_WriteOpl2(byte addr, byte value)
{
    if(addr >= 0xB0) ymbuf[YMB_SIZE - 0xB0 + addr] = value;
    header_ptr->FT_WriteOpl2_ptr(addr, value);
}

void FT_Set4OPMode(uint8_t channel, uint8_t value)
{
    if(SlotX[channel + 20] <= 2)
    {
        OP4[channel] = value;

        if(value)
        {
            if(channel > 10) 
                OP_MASK |= (0x01 << (channel - 11 + 3));
            else 
                OP_MASK |= (0x01 << channel);
        }
        else
        {
            if(channel > 10) 
                OP_MASK &= (0xFF - (0x01 << (channel - 11 + 3)));
            else 
                OP_MASK &= (0xFF -(0x01 << channel));
        }

        header_ptr->FT_WriteOpl2_ptr(0x04, OP_MASK);
    }
}

void FT_SetFreq(uint8_t voice, uint8_t note, int16_t pitch, uint8_t keyOn)
{
    int16_t temp;
    uint16_t fN, divFactor, fNIndex;

    temp = (int16_t)((pitch - 100) / 3.125) + ((note - 12) << LOG_NB_STEP_PITCH);

    if (temp < 0)
        temp = 0;
    else {
        if (temp >= ((NB_NOTES << LOG_NB_STEP_PITCH) - 1))
            temp = (NB_NOTES << LOG_NB_STEP_PITCH) - 1;
    }

    fNIndex = (MOD12[(temp >> 5)] << 5) + (temp & (NB_STEP_PITCH - 1));

    fN = fNumTbl[fNIndex];

    divFactor = DIV12[(temp >> 5)];

    if (voice <= 10)
        FT_WriteOpl1(0xA0 + voice, (fN & 0xFF));
    else
        FT_WriteOpl2(0xA0 + voice - 11, (fN & 0xFF));

    fN = (((fN >> 8) & 0x03) | (divFactor << 2) | keyOn) & 0xFF;

    if (voice <= 10)
        FT_WriteOpl1(0xB0 + voice, fN);
    else
        FT_WriteOpl2(0xB0 + voice - 11, fN);
}

void FT_SetPercussionMode(boolean mode)
{
    if (mode) 
    {
        /* set the frequency for the last 4 percussion voices */
        voiceNote[TOM] = SOP_TOM_PITCH;
        vPitchBend[TOM] = 100;
        FT_SetFreq(TOM, voiceNote[TOM], vPitchBend[TOM], 0);

        voiceNote[SD] = SOP_SD_PITCH;
        vPitchBend[SD] = 100;
        FT_SetFreq(SD, voiceNote[SD], vPitchBend[SD], 0);
    }

    percussion = mode;

    FT_WriteOpl1(0xBD, (percussion ? 0x20 : 0));
}

void FT_NoteOn(uint8_t channel, uint8_t pitch)
{
    if(percussion && channel >= BD && channel <= HIHAT)
    {
        if(channel == BD)
        {
            voiceNote[BD] = pitch;
            FT_SetFreq(BD, voiceNote[BD], vPitchBend[BD], 0);
        }
        else
        {
            if(channel == TOM && (unsigned)voiceNote[TOM] != pitch)
            {
                voiceNote[SD] = (voiceNote[TOM] = pitch) + TOM_TO_SD;
                FT_SetFreq(TOM, voiceNote[TOM], 100, 0);
                FT_SetFreq(SD, voiceNote[SD], 100, 0);
            }
        }
        FT_WriteOpl1(0xBD, ymbuf[0x0D] | (0x10 >> (channel - BD)));
    }
    else
    {
        voiceNote[channel] = pitch;
        voiceKeyOn[channel] = 0x20;

        FT_SetFreq(channel, pitch, vPitchBend[channel], 0x20);
    }
}

//
// Switch off active note
//
void FT_NoteOff(uint8_t channel)
{
    voiceKeyOn[channel] = 0;

    if(percussion && channel >= BD && channel <= HIHAT)
        FT_WriteOpl1(0xBD, ymbuf[0x0D] & (0xFF - (0x10 >> (channel -BD))));
    else
    {
        if(channel < HIHAT)
            FT_WriteOpl1(0xB0 + channel, ymbuf[channel] & 0xDF);
        else
            FT_WriteOpl2(0xB0 - 11 + channel, ymbuf[channel - 11 + YMB_SIZE] & 0xDF);
    }
}

void FT_SetVoiceVolume(uint8_t chan, uint8_t vol)
{    
    uint16_t volume;
    unsigned char KSL_value;

    if (chan > 2 && OP4[chan - 3])
        return;

    header_ptr->FT_SetSegment_ptr(VOLUME_SEGMENT);

    if (vol > MAX_VOLUME)
        vol = MAX_VOLUME;

    VoiceVolume[chan] = vol;

    if (Ksl2V[chan]) {
        volume = 63 - VolumeTable[((63 - ((KSL_value = Ksl2[chan]) & 0x3F)) << 7) + vol];

        if (chan >= 11)
            FT_WriteOpl2(VolReg[chan - 11] - 3, KSL_value & 0xC0 | volume);
        else
            FT_WriteOpl1((percussion ? VolReg[chan + 11] : VolReg[chan]) - 3, KSL_value & 0xC0 | volume);

        if (OP4[chan]) {
            chan += 3;
            volume = 63 - VolumeTable[((63 - ((KSL_value = Ksl[chan]) & 0x3F)) << 7) + vol];

            if (chan >= 11)
                FT_WriteOpl2(VolReg[chan - 11], KSL_value & 0xC0 | volume);
            else
                FT_WriteOpl1(VolReg[chan], KSL_value & 0xC0 | volume);

            if (Ksl2V[chan]) {
                volume = 63 - VolumeTable[((63 - ((KSL_value = Ksl2[chan]) & 0x3F)) << 7) + vol];

                if (chan >= 11)
                    FT_WriteOpl2(VolReg[chan - 11] - 3, KSL_value & 0xC0 | volume);
                else
                    FT_WriteOpl1(VolReg[chan] - 3, KSL_value & 0xC0 | volume);
            }
        }
        else {
            volume = 63 - VolumeTable[((63 - ((KSL_value = Ksl[chan]) & 0x3F)) << 7) + vol];

            if (chan >= 11)
                FT_WriteOpl2(VolReg[chan - 11], KSL_value & 0xC0 | volume);
            else
                FT_WriteOpl1((percussion ? VolReg[chan + 11] : VolReg[chan]), KSL_value & 0xC0 | volume);
        }
    }
    else {
        if (OP4[chan]) {
            volume = 63 - VolumeTable[((63 - ((KSL_value = Ksl[chan + 3]) & 0x3F)) << 7) + vol];

            if (chan >= 11)
                FT_WriteOpl2(VolReg[chan + 3 - 11], KSL_value & 0xC0 | volume);
            else
                FT_WriteOpl1(VolReg[chan + 3], KSL_value & 0xC0 | volume);

            if (Ksl2V[chan + 3]) {
                volume = 63 - VolumeTable[((63 - ((KSL_value = Ksl[chan]) & 0x3F)) << 7) + vol];

                if (chan >= 11)
                    FT_WriteOpl2(VolReg[chan - 11], KSL_value & 0xC0 | volume);
                else
                    FT_WriteOpl1(VolReg[chan], KSL_value & 0xC0 | volume);
            }
        }
        else {
            volume = 63 - VolumeTable[((63 - ((KSL_value = Ksl[chan]) & 0x3F)) << 7) + vol];

            if (chan >= 11)
                FT_WriteOpl2(VolReg[chan - 11], KSL_value & 0xC0 | volume);
            else
                FT_WriteOpl1((percussion ? VolReg[chan + 11] : VolReg[chan]), KSL_value & 0xC0 | volume);
        }
    }   

    header_ptr->FT_SetSegment_ptr(tracks[chan].currentSegment);
}

void FT_SendIns(uint16_t base_addr, uint8_t* value, boolean mode)
{
    uint8_t i;

    for(i = 0; i < 4; i++)
    {
        if(!mode) 
            FT_WriteOpl1(base_addr, *value++);
        else
            FT_WriteOpl2(base_addr, *value++);
        base_addr += 0x20;
    }
    base_addr += 0x40;
    if(!mode) 
        FT_WriteOpl1(base_addr, (*value) & 0x07);
    else
        FT_WriteOpl2(base_addr, (*value) & 0x07);
}

void FT_SetVoiceTimbre(uint8_t chan, uint8_t* array)
{
    uint16_t i;
    uint16_t Slot_Number, KSL_value;

    if (chan > 2 && OP4[chan - 3])
        return;

    header_ptr->FT_SetSegment_ptr(INSTRUMENT_SEGMENT);

    if (!percussion)
        Slot_Number = SlotX[chan];
    else
        Slot_Number = SlotX[chan + 20];

    Ksl2V[chan] = ((KSL_value = (array[5] & 0x0F)) & 0x01);

    if (chan > 10) 
    {
        i = chan + 0xC0 - 11;

        FT_WriteOpl2(i, 0);

        FT_SendIns(0x20 + Slot_Number, array, true);
        FT_SendIns(0x23 + Slot_Number, &array[6], true);

        if (OP4[chan]) {
            FT_WriteOpl2(i + 3, 0);

            FT_SendIns(0x28 + Slot_Number, &((unsigned char*)array)[11], true);
            FT_SendIns(0x2B + Slot_Number, &((unsigned char*)array)[17], true);

            Ksl[chan + 3] = *(array + 18);
            Ksl2[chan + 3] = *(array + 12);
            Ksl2V[chan + 3] = *(array + 16) & 1;

            FT_WriteOpl2(i + 3, (*(array + 16) & 0x0F) | Stereo[chan]);
        }

        Ksl[chan] = *(array + 7);
        Ksl2[chan] = *(array + 1);
        Ksl2V[chan] = *(array + 5) & 1;

        FT_SetVoiceVolume(chan, VoiceVolume[chan]);
        FT_WriteOpl2(i, KSL_value | Stereo[chan]);
    }
    else {
        if (chan > 8)
            i = 0xC0 + 17 - chan;
        else
            i = chan + 0xC0;

        FT_WriteOpl1(i, 0);

        FT_SendIns(0x20 + Slot_Number, array, false);

        if (percussion && chan > BD)
        {
            Ksl[chan] = *(array + 1);
            Ksl2V[chan] = 0;
        }
        else
        {
            FT_SendIns(0x23 + Slot_Number, &array[6], false);

            Ksl[chan] = *(array + 7);
            Ksl2[chan] = *(array + 1);
            Ksl2V[chan] = *(array + 5) & 1;
        }

        if (OP4[chan]) 
        {
            FT_WriteOpl1(i + 3, 0);

            FT_SendIns(0x28 + Slot_Number, &((unsigned char*)array)[11], false);
            FT_SendIns(0x2B + Slot_Number, &((unsigned char*)array)[17], false);

            Ksl[chan + 3] = *(array + 18);
            Ksl2[chan + 3] = *(array + 12);
            Ksl2V[chan + 3] = *(array + 16) & 1;

            FT_WriteOpl1(i + 3, *(array + 16) & 0x0F | Stereo[chan]);
        }

        FT_SetVoiceVolume(chan, VoiceVolume[chan]);
        FT_WriteOpl1(i, KSL_value | Stereo[chan]);
    }

    header_ptr->FT_SetSegment_ptr(tracks[chan].currentSegment);
}

void FT_SetStereoPAN(uint8_t chan, uint8_t value)
{
    uint8_t PAN[] = { 0xA0, 0x30, 0x50 };
    boolean output3 = false;
    uint8_t addr = 0;
    uint8_t data;

    Stereo[chan] = (byte)(value = PAN[value]);

    if(chan < 9) addr = chan;
    else
    {
        if(chan < 11) addr = 17 - chan;
        else
        {
            addr = chan - 11;
            output3 = true;
        }
    }

    value |= ((chan >= 11) ? ymbuf[YMB_SIZE + 0x10 + addr] & 0x0F : ymbuf[addr + 0x10] & 0x0F);

    if(OP4[chan])
    {
        data = (value & 0xF0) | ((chan >= 11) ? ymbuf[YMB_SIZE + 0x13 + addr] : ymbuf[addr + 0x13]) & 0x0F;
        (output3) ? FT_WriteOpl2(addr + 0xC3, data) : FT_WriteOpl1(addr + 0xC3, data);
    }
    (output3) ? FT_WriteOpl2(addr + 0xC0, value) : FT_WriteOpl1(addr + 0xC0, value);
}

void FT_SetVoicePitch(uint8_t chan, uint8_t pitch)
{
    vPitchBend[chan] = pitch;

    if(!percussion)
    {
        FT_SetFreq(chan, voiceNote[chan], pitch, voiceKeyOn[chan]);
    }
    else
    {
        if(chan <= BD || chan > HIHAT)
        {
            FT_SetFreq(chan, voiceNote[chan], pitch, voiceKeyOn[chan]);
        }
    }
}

uint8_t FT_GetTrackData(uint8_t track)
{   
    uint8_t data = *(tracks[track].currentData++);

    if(tracks[track].currentData == (uint8_t *)(SOP_EVNT_BASE + PAGE_SIZE))
    {
        tracks[track].currentSegment++;
        header_ptr->FT_SetSegment_ptr(tracks[track].currentSegment);
        tracks[track].currentData = (uint8_t*)SOP_EVNT_BASE;
    }
    return data;
}
