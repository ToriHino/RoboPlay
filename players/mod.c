//
// RoboPlay
//
// Copyright (C) 2020 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// MOD: Amiga MOD player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "mod.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

boolean RP_Load(char* fileName)
{  
    uint8_t i, maxPattern;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;

    iRoboPlay->RP_Open(fileName);
    iRoboPlay->RP_Read(&modHeader, sizeof(MOD_HEADER));

    moduleType = 0;
    for(i = 0; i < sizeof(fileFormats) / sizeof(MOD_FILE_FORMAT); i++)
    {
        if(modHeader.moduleType[0] == fileFormats[i].id[0] && 
           modHeader.moduleType[1] == fileFormats[i].id[1] &&
           modHeader.moduleType[2] == fileFormats[i].id[2] &&
           modHeader.moduleType[3] == fileFormats[i].id[3])
        {
            moduleType = i;
            break;
        }
    }

    maxPattern = 0;
    for(i = 0; i < 128; i++)
    {
        if(modHeader.patternTable[i] > maxPattern) maxPattern = modHeader.patternTable[i];
    }

    FT_ReadPatterns(maxPattern+1);
    FT_ReadSamples();

    iRoboPlay->RP_Close();

    return true;
}

boolean RP_Update()
{
    uint8_t channel;

    speedStep--;

    if(!speedStep)
    {
        speedStep = speed;
      
        if(command_EE)
        {
            command_EE--;
            return true;
        }

        FT_NextPatternStep();

        for(channel = 0; channel < fileFormats[moduleType].nrOfChannels; channel++)
        {
            command_E5 = 0;
            FT_SetSample(channel);
            FT_HandleCommand(channel);

            if(!( (stepData[channel].effectCommand == 0x0E) && (stepData[channel].extendedCommand == 0x0D) ))
            {
                FT_NewNote(channel);

                if(stepData[channel].effectCommand == 0x0E)
                {
                    switch(stepData[channel].extendedCommand)
                    {
                        case 0x01:
                            channelInfo[channel].period -= stepData[channel].effectData;
                            if(channelInfo[channel].period < 108) channelInfo[channel].period = 108;
                            FT_WAVE_SetFrequency(channel, channelInfo[channel].period);              
                            break;
                        case 0x02:
                            channelInfo[channel].period += stepData[channel].effectData;
                            if(channelInfo[channel].period > 907) channelInfo[channel].period = 907;
                            FT_WAVE_SetFrequency(channel, channelInfo[channel].period);               
                            break;
                    }
                }
            }

            FT_WAVE_SetTotalLevel(channel, channelInfo[channel].volume);
            FT_WAVE_SetKeyOn(channel, true);
        }
    }
    else
    {
        arpeggio = (arpeggio < 2) ? arpeggio + 1 : 0;

        for(channel = 0; channel < fileFormats[moduleType].nrOfChannels; channel++)
        {
            FT_HandleOffNoteCommand(channel);
        }
    }

    return true;
}

void RP_Rewind(int8_t subsong)
{
    uint8_t i;

    // No subsongs in this format
    subsong;

    for(i = 0; i < fileFormats[moduleType].nrOfChannels; i++)
    {
        channelInfo[i].sampleNumber = 0;
        channelInfo[i].oldSampleNumber = 0;
        channelInfo[i].noteNumber = 0;
        channelInfo[i].period = 0;
        channelInfo[i].volume = 0;
        channelInfo[i].tempCommand = 0;
        channelInfo[i].pitchBendSpeed = 0;
        channelInfo[i].vibratoCommand = 0;
        channelInfo[i].vibratoTablePosition = 0;
        channelInfo[i].tremoloCommand = 0;
        channelInfo[i].tremoloTablePosition = 0;
        channelInfo[i].waveControl = 0;
        channelInfo[i].patternLoopStart = 0;
        channelInfo[i].patternLoopCount = 0;
    }

    bpm = 125;
    speed = 6;
    speedStep = 1;

    songPosition = 255;
    patternStep = MAX_PATTERN_STEP;

    arpeggio = 0;
    command_E5 = 0;
    command_EE = 0;
}

float RP_GetRefresh()
{ 
    float refresh = bpm * 0.4;
    return refresh;

}

uint8_t RP_GetSubSongs()
{
    return 0;
}

char* RP_GetPlayerInfo()
{
    return "Amiga Module player by RoboSoft Inc.";
}

char* RP_GetTitle()
{
    return modHeader.songName;
}

char* RP_GetAuthor()
{
    return "-";
}

char* RP_GetDescription()
{
    return fileFormats[moduleType].description;
}

void FT_ReadPatterns(uint8_t nrOfPatterns)
{
    uint8_t  i;
    uint8_t* fileData = (uint8_t*)SEGMENT_BASE;
    uint16_t pageLeft = SEGMENT_SIZE;
    uint8_t  currentSegment;

    currentSegment = iRoboPlay->RP_AllocateSegment();
    iRoboPlay->RP_SetSegment(currentSegment);

    patternSize = fileFormats[moduleType].nrOfChannels * STEP_DATA_SIZE * (MAX_PATTERN_STEP+1);

    for(i = 0; i < nrOfPatterns; i++)
    {
        patternInfo[i].segment = currentSegment;
        patternInfo[i].address = fileData;

        // It's not possible to read directly to non-primary mapper memory segments,
        // so use a buffer inbetween.
        iRoboPlay->RP_Read((void*)READ_BUFFER, patternSize);
        memcpy(fileData, (void*)READ_BUFFER, patternSize);

        fileData += patternSize;
        pageLeft = (pageLeft > patternSize) ? pageLeft - patternSize : 0;

        if(!pageLeft)
        {
            currentSegment = iRoboPlay->RP_AllocateSegment();
            iRoboPlay->RP_SetSegment(currentSegment);

            pageLeft = SEGMENT_SIZE;
            fileData = (uint8_t*)SEGMENT_BASE;
        }
    }
}

void FT_ReadSamples()
{
    uint8_t  i;
    uint16_t bytesLeft, bytesRead, readSize;

    uint32_t waveAddress = OPL_WAVE_MEMORY + 384 * 12;
    uint16_t headerAddress = 0;
    
    uint32_t start;
    uint16_t loop, end;

    // Set OPL4 to memory access mode
    iRoboPlay->RP_WriteWave(0x02, 0x11);

    for(i = 0; i < NR_OF_SAMPLES; i++)
    {
        modHeader.sampleInfo[i].sampleLength = ((modHeader.sampleInfo[i].sampleLength << 8) | (modHeader.sampleInfo[i].sampleLength >> 8)) * 2;
        modHeader.sampleInfo[i].sampleRepeatPoint = ((modHeader.sampleInfo[i].sampleRepeatPoint << 8) | (modHeader.sampleInfo[i].sampleRepeatPoint >> 8)) * 2;
        modHeader.sampleInfo[i].sampleRepeatLength = ((modHeader.sampleInfo[i].sampleRepeatLength << 8) | (modHeader.sampleInfo[i].sampleRepeatLength >> 8)) * 2;

        start = waveAddress;
        loop  = modHeader.sampleInfo[i].sampleLength - 1;
        end   = (modHeader.sampleInfo[i].sampleLength - 1) ^ 0xFFFF;

        if(modHeader.sampleInfo[i].sampleRepeatPoint > 0 || modHeader.sampleInfo[i].sampleRepeatLength > 4)
        {
            loop = modHeader.sampleInfo[i].sampleRepeatPoint;
            end = (loop + modHeader.sampleInfo[i].sampleRepeatLength - 1) ^ 0xFFFF;
        }

        // Write header to OPL memory
        iRoboPlay->RP_WriteWave(3, 0x20);
        iRoboPlay->RP_WriteWave(4, (headerAddress >> 8) & 0xFF);
        iRoboPlay->RP_WriteWave(5, headerAddress & 0xFF);

        // Start address
        iRoboPlay->RP_WriteWave(6, (start >> 16) & 0x3F);
        iRoboPlay->RP_WriteWave(6, (start >> 8) & 0xFF);
        iRoboPlay->RP_WriteWave(6, start & 0xFF);

        // Loop address
        iRoboPlay->RP_WriteWave(6, (loop >> 8) & 0xFF);
        iRoboPlay->RP_WriteWave(6, loop & 0xFF);

        // End address
        iRoboPlay->RP_WriteWave(6, (end >> 8) & 0xFF);
        iRoboPlay->RP_WriteWave(6, end & 0xFF);


        iRoboPlay->RP_WriteWave(6, 0);          // LFO, VIB        
        iRoboPlay->RP_WriteWave(6, 0xF0);       // AR, D1R
        iRoboPlay->RP_WriteWave(6, 0xFF);       // DL, D2R      
        iRoboPlay->RP_WriteWave(6, 0x0F);       // Rate correction , RR
        iRoboPlay->RP_WriteWave(6, 0);          // AM

        headerAddress += 12;

        // Set wave memory adress
        iRoboPlay->RP_WriteWave(3, (waveAddress >> 16) & 0x3F);
        iRoboPlay->RP_WriteWave(4, (waveAddress >> 8)  & 0xFF);
        iRoboPlay->RP_WriteWave(5, waveAddress & 0xFF);

        bytesLeft = modHeader.sampleInfo[i].sampleLength;
        while(bytesLeft)
        {
            readSize = (bytesLeft > READ_BUFFER_SIZE) ? READ_BUFFER_SIZE : bytesLeft;
            bytesRead = iRoboPlay->RP_Read((void*)READ_BUFFER, readSize);

            iRoboPlay->RP_WriteWaveData((uint8_t*)READ_BUFFER, bytesRead);

            waveAddress += bytesRead;
            bytesLeft -= bytesRead;
        }
    }

    // Set OPL4 to sound generation mode
    iRoboPlay->RP_WriteWave(0x2, 0x10);
}

void FT_NextPatternStep()
{
    patternStep++;

    if(patternStep > MAX_PATTERN_STEP)
    {
        songPosition = (songPosition < (modHeader.songLength-1)) ? songPosition + 1 : 0;
        patternStep = 0;

        FT_SetPatternSegment();
    }

    FT_UpdateStepData();
}

void FT_SetPatternSegment()
{
    iRoboPlay->RP_SetSegment(patternInfo[modHeader.patternTable[songPosition]].segment);
}

void FT_UpdateStepData()
{
    int      channel;
    uint8_t* currentStep;

    currentStep = patternInfo[modHeader.patternTable[songPosition]].address + (patternStep * (fileFormats[moduleType].nrOfChannels * STEP_DATA_SIZE));
    for(channel = 0; channel < fileFormats[moduleType].nrOfChannels; channel++)
    {
        stepData[channel].sampleNumber    = (currentStep[0] & 0xF0) + ((currentStep[2] >> 4) & 0x0F);
        stepData[channel].period          = ((currentStep[0] & 0x0F) << 8) + currentStep[1];
        stepData[channel].effectCommand   = currentStep[2] & 0x0F;
        stepData[channel].effectData      = currentStep[3];
        if(stepData[channel].effectCommand == 0x0E)
        {
            stepData[channel].extendedCommand = stepData[channel].effectData >> 4;
            stepData[channel].effectData &= 0x0F;
        }
        currentStep += STEP_DATA_SIZE;
    }
}

void FT_HandleCommand(uint8_t channel)
{
    switch(stepData[channel].effectCommand)
    {
        case 0x00:
            arpeggio = 0;
            FT_WAVE_SetFrequency(channel, channelInfo[channel].period);
            break;
        case 0x04:
            channelInfo[channel].tempCommand = channelInfo[channel].period;

            if(stepData[channel].effectData & 0xF0)
            {
                channelInfo[channel].vibratoCommand &= 0x0F;
                channelInfo[channel].vibratoCommand |= (stepData[channel].effectData & 0xF0);
            }
            if(stepData[channel].effectData & 0x0F)
            {
                channelInfo[channel].vibratoCommand &= 0xF0;
                channelInfo[channel].vibratoCommand |= (stepData[channel].effectData & 0x0F);
            }
            break;
        case 0x07:
            if(stepData[channel].effectData & 0xF0)
            {
                channelInfo[channel].tremoloCommand &= 0x0F;
                channelInfo[channel].tremoloCommand |= (stepData[channel].effectData & 0xF0);
            }
            if(stepData[channel].effectData & 0x0F)
            {
                channelInfo[channel].tremoloCommand &= 0xF0;
                channelInfo[channel].tremoloCommand |= (stepData[channel].effectData & 0x0F);
            }
            break;
        case 0x0C:
            channelInfo[channel].volume = (stepData[channel].effectData > 64) ? 64 : stepData[channel].effectData;
            FT_WAVE_SetTotalLevel(channel, channelInfo[channel].volume);
            break;
        case 0x0E:
            switch(stepData[channel].extendedCommand)
            {
                case 0x04:
                    channelInfo[channel].waveControl &= 0xFC;
                    channelInfo[channel].waveControl |= stepData[channel].effectData;
                    channelInfo[channel].vibratoTablePosition = 0;
                    break;
                case 0x05:
                    command_E5 = stepData[channel].effectData;
                    break;
                case 0x07:
                    channelInfo[channel].waveControl &= 0xF3;
                    channelInfo[channel].waveControl |= (stepData[channel].effectData << 2);
                    channelInfo[channel].tremoloTablePosition = 0;
                    break;
                case 0x09:
                    channelInfo[channel].tempCommand = stepData[channel].effectData;
                    break;
                case 0x0A:
                    channelInfo[channel].volume += stepData[channel].effectData;
                    if(channelInfo[channel].volume > MAX_VOLUME)
                        channelInfo[channel].volume = MAX_VOLUME;
                    FT_WAVE_SetTotalLevel(channel, channelInfo[channel].volume);
                    break;
                case 0x0B:
                    if(channelInfo[channel].volume >= stepData[channel].effectData)
                        channelInfo[channel].volume -= stepData[channel].effectData;
                    else
                        channelInfo[channel].volume = 0;              
                    FT_WAVE_SetTotalLevel(channel, channelInfo[channel].volume);
                    break;
                case 0x0C:
                    if(stepData[channel].effectData)
                        channelInfo[channel].tempCommand = stepData[channel].effectData;
                    else
                    {
                        channelInfo[channel].volume = 0;
                        FT_WAVE_SetTotalLevel(channel, channelInfo[channel].volume);                        
                    }
                    break;
                case 0x0D:
                    if(stepData[channel].effectData)
                        channelInfo[channel].tempCommand = stepData[channel].effectData;                  
                    break;
                case 0x0E:
                    command_EE = stepData[channel].effectData;
                    break;
                case 0x0F:
                    break;
            }
            break;
        case 0x0F:
            if(stepData[channel].effectData < 32)
            {
                speed = stepData[channel].effectData;
                speedStep = speed;
            }
            else
            {
                bpm = stepData[channel].effectData;
                iRoboPlay->RP_UpdateRefresh();
            }
            break;
    }
}

void FT_HandleOffNoteCommand(uint8_t channel)
{
    switch(stepData[channel].effectCommand)
    {
        case 0x00:
            if(stepData[channel].effectData)
            {
                uint8_t  noteNumber, newNoteNumber;

                if(!arpeggio) 
                {
                    FT_WAVE_SetFrequency(channel, channelInfo[channel].period);
                }
                else
                {
                    noteNumber = channelInfo[channel].noteNumber;

                    if(arpeggio == 1)
                        newNoteNumber = noteNumber + (stepData[channel].effectData >> 4);
                    else
                        newNoteNumber = noteNumber + (stepData[channel].effectData & 0x0F);

                    FT_WAVE_SetFrequency(channel, FT_TunePeriod(channel, periods[newNoteNumber]));
                    channelInfo[channel].noteNumber = noteNumber;
                }
            }
            break;
        case 0x01:
            channelInfo[channel].period -= stepData[channel].effectData;
            if(channelInfo[channel].period < 108) channelInfo[channel].period = 108;
            FT_WAVE_SetFrequency(channel, channelInfo[channel].period);
            break;
        case 0x02:
            channelInfo[channel].period += stepData[channel].effectData;
            if(channelInfo[channel].period > 907) channelInfo[channel].period = 907;
            FT_WAVE_SetFrequency(channel, channelInfo[channel].period);
            break;
        case 0x03:
            FT_TonePortamento(channel);
            break;
        case 0x04:
            FT_Vibrato(channel);
            break;
        case 0x05:
            FT_TonePortamento(channel);
            FT_VolumeSlide(channel);
            break;
        case 0x06:
            FT_Vibrato(channel);
            FT_VolumeSlide(channel);
            break;
        case 0x07:
            FT_Tremolo(channel);
            break;
        case 0x0A:
            FT_VolumeSlide(channel);
            break;
        case 0x0B:
            if(speedStep == 1)
            {
                songPosition = stepData[channel].effectData - 1;
                patternStep = MAX_PATTERN_STEP;
            }
            break;
        case 0x0D:
            if(speedStep == 1)
            {
                songPosition++;
                patternStep = stepData[channel].effectData - 1;
                FT_SetPatternSegment();
            }
            break;
        case 0x0E:
            switch(stepData[channel].extendedCommand)
            {
                case 0x06:
                    if(speedStep == 1)
                    {
                        if(!channelInfo[channel].tempCommand)
                        {
                            channelInfo[channel].patternLoopStart = patternStep - 1;
                            channelInfo[channel].patternLoopCount = 0;
                        }
                        else
                        {
                            if(channelInfo[channel].patternLoopCount < channelInfo[channel].tempCommand)
                            {
                                channelInfo[channel].patternLoopCount++;
                                patternStep = channelInfo[channel].patternLoopStart;
                            }
                        }
                    }
                    break;
                case 0x09:
                    FT_NewNote(channel);
                    break;
                case 0x0C:
                    if(channelInfo[channel].tempCommand)
                    {
                        channelInfo[channel].tempCommand--;
                        if(!channelInfo[channel].tempCommand)
                        {
                            channelInfo[channel].volume = 0;
                            FT_WAVE_SetTotalLevel(channel, channelInfo[channel].volume);                          
                        }
                    }
                    break;
                case 0x0D:
                    if(channelInfo[channel].tempCommand)
                    {
                        channelInfo[channel].tempCommand--;
                        if(!channelInfo[channel].tempCommand)
                            FT_NewNote(channel);
                    }
                    break;
            }
            break;
    }
}

void FT_SetSample(uint8_t channel)
{
    if(stepData[channel].sampleNumber)
    {
        channelInfo[channel].sampleNumber = stepData[channel].sampleNumber;
        channelInfo[channel].volume = modHeader.sampleInfo[stepData[channel].sampleNumber - 1].volume;
        channelInfo[channel].vibratoTablePosition = 0;
    }
}

void FT_NewNote(uint8_t channel)
{
    if(channelInfo[channel].sampleNumber)
    {
        if(stepData[channel].effectCommand == 0x03 || stepData[channel].effectCommand == 0x05)
        {
            if(stepData[channel].effectCommand == 0x03 && stepData[channel].effectData)
                channelInfo[channel].pitchBendSpeed = stepData[channel].effectData;

            if(stepData[channel].period)
                channelInfo[channel].tempCommand = FT_TunePeriod(channel, stepData[channel].period);
        }
        else if(stepData[channel].period)
        {          
            FT_WAVE_SetTotalLevel(channel, 0);
            FT_WAVE_SetKeyOn(channel, false);
            if(channelInfo[channel].sampleNumber != channelInfo[channel].oldSampleNumber)
            {
                channelInfo[channel].oldSampleNumber = channelInfo[channel].sampleNumber;
                FT_WAVE_SetSampleNumber(channel, channelInfo[channel].sampleNumber);
            }
            
            channelInfo[channel].period = FT_TunePeriod(channel, stepData[channel].period);
            FT_WAVE_SetFrequency(channel, channelInfo[channel].period);
        }
    }
}

uint16_t FT_TunePeriod(uint8_t channel, uint16_t period)
{
    uint8_t i;
    for(i = 0; i < NOTE_NUMBERS; i++)
    {
        if(periods[i] == period) break;
    }
    channelInfo[channel].noteNumber = i;

    if(command_E5)
    {
        period += finetuneTable[command_E5 - 1][channelInfo[channel].noteNumber];
    }
    else if((modHeader.sampleInfo[channelInfo[channel].sampleNumber - 1].finetune & 0x0F)) 
    {  
        uint8_t finetune = (modHeader.sampleInfo[channelInfo[channel].sampleNumber - 1].finetune & 0x0F);
        period += finetuneTable[finetune - 1][channelInfo[channel].noteNumber];
    }

    return period;
}

void FT_TonePortamento(uint8_t channel)
{
    if(channelInfo[channel].period > channelInfo[channel].tempCommand)
    {
        if(channelInfo[channel].period >= channelInfo[channel].pitchBendSpeed)
            channelInfo[channel].period -= channelInfo[channel].pitchBendSpeed;
        else
            channelInfo[channel].period = 0;

        if(channelInfo[channel].period < channelInfo[channel].tempCommand)
            channelInfo[channel].period = channelInfo[channel].tempCommand;
    }
    if(channelInfo[channel].period < channelInfo[channel].tempCommand)
    {
        channelInfo[channel].period += channelInfo[channel].pitchBendSpeed;   
        if(channelInfo[channel].period > channelInfo[channel].tempCommand)
            channelInfo[channel].period = channelInfo[channel].tempCommand;
    }

     FT_WAVE_SetFrequency(channel, channelInfo[channel].period);
}

void FT_Vibrato(uint8_t channel)
{
    uint8_t vibIndex;
    uint16_t vibValue;

    vibIndex = (channelInfo[channel].vibratoTablePosition >> 2) & 0x1F;
    vibValue = vibratoTable[vibIndex] * (channelInfo[channel].vibratoCommand & 0x0F);
    vibValue = vibValue >> 7;

    if(channelInfo[channel].vibratoTablePosition > 127)
        channelInfo[channel].tempCommand = channelInfo[channel].period - vibValue;
    else
        channelInfo[channel].tempCommand = channelInfo[channel].period + vibValue;

    channelInfo[channel].vibratoTablePosition += ((channelInfo[channel].vibratoCommand >> 2) & 0x3C);

    FT_WAVE_SetFrequency(channel, channelInfo[channel].tempCommand);
}

void FT_Tremolo(uint8_t channel)
{
    uint8_t tremIndex, volume;
    uint16_t tremValue;

    tremIndex = (channelInfo[channel].tremoloTablePosition >> 2) & 0x1F;
    tremValue = vibratoTable[tremIndex] * (channelInfo[channel].tremoloCommand & 0x0F);
    tremValue = tremValue >> 6;

    if(channelInfo[channel].tremoloTablePosition > 127)
    {
        volume = (channelInfo[channel].volume >= tremValue) ?  channelInfo[channel].volume - tremValue : 0;
    }
    else
    {
        volume = channelInfo[channel].volume + tremValue;
        if(volume > 64) volume = 64;
    }

    channelInfo[channel].tremoloTablePosition += ((channelInfo[channel].tremoloCommand >> 2) & 0x3C);

    FT_WAVE_SetTotalLevel(channel, volume);
}

void FT_VolumeSlide(uint8_t channel)
{
    if(stepData[channel].effectData < 0x10)
    {
        if(channelInfo[channel].volume >= stepData[channel].effectData & 0x0F)
            channelInfo[channel].volume -= stepData[channel].effectData & 0x0F;
        else
            channelInfo[channel].volume = 0;              
    }
    else
    {
        channelInfo[channel].volume += ((stepData[channel].effectData >> 4) & 0x0F);
        if(channelInfo[channel].volume > MAX_VOLUME)
            channelInfo[channel].volume = MAX_VOLUME;
    }

    FT_WAVE_SetTotalLevel(channel, channelInfo[channel].volume);
}

void FT_WAVE_SetSampleNumber(uint8_t channel, uint8_t sampleNumber)
{
    iRoboPlay->RP_WriteWave(0x08 + channel, 0x7F + sampleNumber);
    while ((iRoboPlay->RP_ReadOpl() & 0x02) == 1);
}

void FT_WAVE_SetFrequency(uint8_t channel, uint16_t period)
{
    uint16_t pitchIndex = 2 * (period - 108);
    iRoboPlay->RP_WriteWave(0x38 + channel, pitchTable[pitchIndex + 1]);
    iRoboPlay->RP_WriteWave(0x20 + channel, pitchTable[pitchIndex] | 0x01);
}

void FT_WAVE_SetTotalLevel(uint8_t channel, uint8_t volume)
{
    iRoboPlay->RP_WriteWave(0x50 + channel, (volumeTable[volume] << 1) | 0x01);
}

void FT_WAVE_SetKeyOn(uint8_t channel, boolean keyOn)
{
    if(!keyOn)
        iRoboPlay->RP_WriteWave(0x68 + channel, 0x7F & panSetting[channel] );
    else
        iRoboPlay->RP_WriteWave(0x68 + channel, 0x80 | panSetting[channel]);
 }