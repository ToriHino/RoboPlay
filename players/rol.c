//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// ROL: AdLib Visual Composer player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "rol.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

boolean RP_Load(char* fileName)
{  
    uint8_t  i;
    uint8_t* filedata;
    uint16_t bytesRead;

    ROL_NOTE* note;
    uint16_t  nTicks;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;
    iRoboPlay->RP_Open(fileName);

    iRoboPlay->RP_Read(&rolHeader, sizeof(ROL_HEADER));

    filedata = (uint8_t*)ROL_FILE_DATA;
    bytesRead = iRoboPlay->RP_Read(filedata, ROL_MAX_FILE_SIZE);
    iRoboPlay->RP_Close();

    if(rolHeader.majorVersion != ROL_MAJOR_VERSION || rolHeader.minorVersion != ROL_MINOR_VERSION)
        return false;

    tempoTrack = (ROL_TEMPO_TRACK*)filedata;
    filedata += sizeof(ROL_TEMPO_TRACK) - 2;
    filedata += tempoTrack->nEvents * sizeof(ROL_TEMPO);

    timeOfLastNote = 0;
    for(i = 0; i < ROL_NR_CHANNELS; i++)
    {  
        voiceData[i].voiceTrack = (ROL_VOICE_TRACK*)filedata;
        filedata += sizeof(ROL_VOICE_TRACK) - 2;

        nTicks = 0;
        do
        {
            note = (ROL_NOTE *)filedata;
            nTicks += note->duration;
            filedata += sizeof(ROL_NOTE);
        } while(nTicks < voiceData[i].voiceTrack->nTicks);

        if(voiceData[i].voiceTrack->nTicks > timeOfLastNote)
        {
            timeOfLastNote = voiceData[i].voiceTrack->nTicks;
        }

        voiceData[i].timbreTrack = (ROL_TIMBRE_TRACK*)filedata;
        filedata += sizeof(ROL_TIMBRE_TRACK) - 2;
        filedata += voiceData[i].timbreTrack->nEvents * sizeof(ROL_TIMBRE);

        voiceData[i].volumeTrack = (ROL_VOLUME_TRACK*)filedata;
        filedata += sizeof(ROL_VOLUME_TRACK) - 2;
        filedata += voiceData[i].volumeTrack->nEvents * sizeof(ROL_VOLUME);

        voiceData[i].pitchTrack = (ROL_PITCH_TRACK*)filedata;
        filedata += sizeof(ROL_PITCH_TRACK) - 2;
        filedata += voiceData[i].pitchTrack->nEvents * sizeof(ROL_PITCH);
    }

    return true;
}

boolean RP_Update()
{
    uint8_t i;

    if(nextTempoEvent < tempoTrack->nEvents && 
        tempoTrack->tempoEvents[nextTempoEvent].atTick == currTick)
    {
        FT_SetRefresh(tempoTrack->tempoEvents[nextTempoEvent].multiplier);
        nextTempoEvent++;
    }

    for(i = 0; i < ROL_NR_CHANNELS; i++)
    {
        FT_UpdateVoice(i);
    }

    currTick++;
    return (currTick > timeOfLastNote) ? false : true;
}

void RP_Rewind(int8_t subsong)
{
    uint8_t i;

    // No subsongs in this format
    subsong;

    for(i = 0; i < ROL_NR_CHANNELS; i++)
    {
        voiceData[i].eventStatusNoteEnd = false;
        voiceData[i].eventStatusPitchEnd = false;
        voiceData[i].eventStatusInstrEnd = false;
        voiceData[i].eventStatusVolumeEnd = false;

        voiceData[i].noteDuration = 0;
        voiceData[i].currentNoteDuration = 0;
  
        voiceData[i].currentNote = 0;
        voiceData[i].nextInstrumentEvent = 0;
        voiceData[i].nextVolumeEvent = 0;
        voiceData[i].nextPitchEvent = 0;
  
        voiceData[i].forceNote = true;
    }

    currTick = 0;
    nextTempoEvent = 0;

    FT_SetRefresh(1.0f);
}

float RP_GetRefresh()
{
    return refresh;
}

uint8_t RP_GetSubSongs()
{
    return 0;
}

char* RP_GetPlayerInfo()
{
    return "AdLib Visual Composer player by RoboSoft Inc.";
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
    return rolHeader.signature;
}

void FT_SetRefresh(const float multiplier)
{
    float tickBeat = (rolHeader.tickBeat) < MAXTICKBEAT ? rolHeader.tickBeat : MAXTICKBEAT;

    refresh = (tickBeat * tempoTrack->basicTempo * multiplier) / 60.0;
    iRoboPlay->RP_UpdateRefresh();
}

void FT_UpdateVoice(const uint8_t voice)
{
    uint8_t volume;

    ROL_VOICE_DATA* data = &voiceData[voice];

    if(!data->voiceTrack->nTicks || data->eventStatusNoteEnd) return;

    if(!data->eventStatusInstrEnd)
    {
        if(data->nextInstrumentEvent < data->timbreTrack->nEvents)   
        {
            if(data->timbreTrack->timbreEvents[data->nextInstrumentEvent].atTick == currTick)
            {
                FT_SendInstData(voice, data->timbreTrack->timbreEvents[data->nextInstrumentEvent].instName);
                data->nextInstrumentEvent++;
            }
        } else data->eventStatusInstrEnd = true;
    }

    if(!data->eventStatusVolumeEnd)
    {
        if(data->nextVolumeEvent < data->volumeTrack->nEvents)
        {
            if(data->volumeTrack->volumeEvents[data->nextVolumeEvent].atTick == currTick)
            {
                volume = (uint8_t)(MAXVOLUME * data->volumeTrack->volumeEvents[data->nextVolumeEvent].volume);
                FT_SetVolume(voice, volume);
                data->nextVolumeEvent++;
            }
        } else data->eventStatusVolumeEnd = true;
    }

    if(data->forceNote || (data->currentNoteDuration > data->noteDuration -1))
    {
        if(!currTick) data->currentNote++;

        if(data->currentNote < data->voiceTrack->nTicks)
        {
            FT_SetNote(voice, data->voiceTrack->noteEvents[data->currentNote].note);
            data->currentNoteDuration = 0;
            data->noteDuration = data->voiceTrack->noteEvents[data->currentNote].duration;
            data->forceNote = false;
        }
        else
        {
            FT_SetNote(voice, SILENCENOTE);
            data->eventStatusNoteEnd = true;
            return;
        }
    }

    if(!data->eventStatusPitchEnd)
    {
        if(data->nextPitchEvent < data->pitchTrack->nEvents)
        {
                if(data->pitchTrack->pitchEvents[data->nextPitchEvent].atTick == currTick)
                {
                    FT_SetPitch(voice, data->pitchTrack->pitchEvents[data->nextPitchEvent].pitch);
                    data->nextPitchEvent++;
                }
        }
        else
        {
            data->eventStatusPitchEnd = true;
        }
    }
}

void FT_SendInstData(const uint8_t voice, char* instName)
{

}

void FT_SetVolume(const uint8_t voice, const uint8_t volume)
{

}

void FT_SetNote(const uint8_t voice, const uint16_t note)
{
    if(voice < BASSDRUMCHANNEL || rolHeader.isMelodic)
        FT_SetNoteMelodic(voice, note);
    else
        FT_SetNotePercussive(voice, note);
}

void FT_SetPitch(const uint8_t voice, const float pitch)
{

}

void FT_SetNoteMelodic(const uint8_t voice, const uint16_t note)
{

}

void FT_SetNotePercussive(const uint8_t voice, const uint16_t note)
{

}
