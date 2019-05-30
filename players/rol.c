//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// ROL: AdLib Visual Composer player

#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "rol.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

const uint16_t note_table[12] =
    {363, 385, 408, 432, 458, 485, 514, 544, 577, 611, 647, 686};

const uint8_t op_table[9] =
    {0x00, 0x01, 0x02, 0x08, 0x09, 0x0a, 0x10, 0x11, 0x12};

const uint8_t drum_op_table[4] = 
    {0x14, 0x12, 0x15, 0x11};

int strcasecmp(const char *s1, const char *s2)
 {
    const unsigned char *us1 = (const unsigned char *)s1,
                        *us2 = (const unsigned char *)s2;

    while (tolower(*us1) == tolower(*us2++))
        if (*us1++ == '\0')
            return (0);
    return (tolower(*us1) - tolower(*--us2));
}

boolean RP_Load(char* fileName)
{  
    uint8_t  i;
    uint8_t* filedata;
    uint8_t* destination;
    uint16_t bytesRead;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;
    iRoboPlay->RP_Open(fileName);

    iRoboPlay->RP_Read(&rolHeader, sizeof(ROL_HEADER));

    currentSegment = 0;
    segmentList[currentSegment] = iRoboPlay->RP_AllocateSegment();
    iRoboPlay->RP_SetSegment(segmentList[currentSegment]);

    filedata = (uint8_t*)ROL_FILE_DATA;
    bytesRead = iRoboPlay->RP_Read(filedata, ROL_MAX_FILE_SIZE);
    iRoboPlay->RP_Close();

    if(rolHeader.majorVersion != ROL_MAJOR_VERSION || rolHeader.minorVersion != ROL_MINOR_VERSION)
        return false;

    tempoTrack = (ROL_TEMPO_TRACK*)filedata;
    filedata += sizeof(ROL_TEMPO_TRACK);
    filedata += tempoTrack->nEvents * sizeof(ROL_TEMPO);

    timeOfLastNote = 0;
    for(i = 0; i < ROL_NR_CHANNELS; i++)
    {  
        voiceData[i].voiceTrack = (ROL_VOICE_TRACK*)filedata;
        filedata += sizeof(ROL_VOICE_TRACK);

        if(voiceData[i].voiceTrack->nTicks)
        {
            uint16_t  nTicks = 0;
            do
            {
                ROL_NOTE* note = (ROL_NOTE *)filedata;
                nTicks += note->duration;
                filedata += sizeof(ROL_NOTE);
            } while(nTicks < voiceData[i].voiceTrack->nTicks);

            if(voiceData[i].voiceTrack->nTicks > timeOfLastNote)
            {
                timeOfLastNote = voiceData[i].voiceTrack->nTicks;
            }
        }

        voiceData[i].timbreTrack = (ROL_TIMBRE_TRACK*)filedata;
        filedata += sizeof(ROL_TIMBRE_TRACK);
        filedata += voiceData[i].timbreTrack->nEvents * sizeof(ROL_TIMBRE);

        voiceData[i].volumeTrack = (ROL_VOLUME_TRACK*)filedata;
        filedata += sizeof(ROL_VOLUME_TRACK);
        filedata += voiceData[i].volumeTrack->nEvents * sizeof(ROL_VOLUME);

        voiceData[i].pitchTrack = (ROL_PITCH_TRACK*)filedata;
        filedata += sizeof(ROL_PITCH_TRACK);
        filedata += voiceData[i].pitchTrack->nEvents * sizeof(ROL_PITCH);
    }

    instSegment = ++currentSegment;
    iRoboPlay->RP_Open("STANDARD.BNK");
    do
    {
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
    } while(bytesRead);
    iRoboPlay->RP_Close();    

    iRoboPlay->RP_SetSegment(segmentList[0]);
    bnkHeader = (BNK_HEADER*)(SEGMENT_BASE);

    FT_FindInstruments();

    return true;
}

boolean RP_Update()
{
    uint8_t i = 0;

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

    for(i =0; i < NUM_PERCUSSIVE_VOICES; i++)
    {
        HalfToneOffset[i] = 0;
        VolumeCache[i] = MAXVOLUME;
        KSLTLCache[i] = 0;
        NoteCache[i] = 0;
        KOnOctFNumCache[i] = 0;
        KeyOnCache[i] = false;

        FNumFreqPtrList[i] = FNumNotes[0];
    }

    nextTempoEvent = 0;
    currTick = 0;
    AMVibRhythmCache = 0;

    PitchRangeStep = NRSTEPPITCH;

    FT_SetRefresh(1.0f);
}

float RP_GetRefresh()
{
//    return refresh;
    return 60.0;
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

void FT_FindInstruments()
{
    uint8_t  voice = 0;
    uint16_t i;

    for(voice = 0; voice < ROL_NR_CHANNELS; voice++)
    {
        ROL_TIMBRE* timbre = voiceData[voice].timbreTrack->timbreEvents;
        for(i = 0; i < voiceData[voice].timbreTrack->nEvents; i++)
        {
            FT_FindInstrument(timbre++);
        }
    }
}

void FT_FindInstrument(ROL_TIMBRE* timbre)
{
    char instName[9];
    uint8_t seg;
    boolean found;
    BNK_INSTRUMENT_HEADER* bnkInstHeader;

    strcpy(instName, timbre->instName);

    seg = instSegment;
    iRoboPlay->RP_SetSegment(segmentList[seg]);

    bnkInstHeader = (BNK_INSTRUMENT_HEADER*)(SEGMENT_BASE + bnkHeader->offsetName);

    found = false;
    while(!found)
    {
        found = !strcasecmp(instName, bnkInstHeader->name);

        if(!found)
        {
            bnkInstHeader++;
            if((uint16_t)bnkInstHeader > (SEGMENT_BASE + SEGMENT_SIZE))
            {
                bnkInstHeader = (BNK_INSTRUMENT_HEADER*)((uint16_t)bnkInstHeader - SEGMENT_SIZE);
                iRoboPlay->RP_SetSegment(segmentList[++seg]);
            }
        }
        else
        {
            uint32_t t = bnkHeader->offsetData + bnkInstHeader->index * sizeof(BNK_INSTRUMENT_HEADER);
            iRoboPlay->RP_SetSegment(segmentList[0]);
            timbre->filler = instSegment;
            while(t > SEGMENT_SIZE)
            {
                t -= SEGMENT_SIZE;
                timbre->filler++;
            }
            timbre->unknown = (SEGMENT_BASE + t);
            break;
        }
    }   
}

void FT_SetRefresh(const float multiplier)
{
//    float tickBeat = (rolHeader.tickBeat) < MAXTICKBEAT ? rolHeader.tickBeat : MAXTICKBEAT;

//    refresh = (tickBeat * tempoTrack->basicTempo * multiplier) / 60.0;
//    iRoboPlay->RP_UpdateRefresh();
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
                FT_SendInstData(
                    voice,
                    data->timbreTrack->timbreEvents[data->nextInstrumentEvent].filler, 
                    (SRolInstrument*)data->timbreTrack->timbreEvents[data->nextInstrumentEvent].unknown);
                
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
                //FT_SetVolume(voice, volume);
                FT_SetVolume(voice, MAXVOLUME);
                data->nextVolumeEvent++;
            }
        } else data->eventStatusVolumeEnd = true;
    }

    if(data->forceNote || (data->currentNoteDuration > data->noteDuration - 1))
    {
        if(data->currentNote < data->voiceTrack->nTicks)
        {
            FT_SetNote(voice, data->voiceTrack->noteEvents[data->currentNote].note);
            data->currentNoteDuration = 0;
            data->noteDuration = data->voiceTrack->noteEvents[data->currentNote].duration;
            data->forceNote = false;
            data->currentNote++;
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

    data->currentNoteDuration++;
}

void FT_SendInstData(const uint8_t voice, const uint8_t segment, const SRolInstrument* instrument)
{
    iRoboPlay->RP_SetSegment(segmentList[segment]);
    FT_SendOperator(voice, &instrument->modulator, &instrument->carrier);
    iRoboPlay->RP_SetSegment(segmentList[0]);
}

uint8_t FT_GetKSLTL(const uint8_t voice)
{
    uint16_t kslTL = OPL2_TLMinLevel - (KSLTLCache[voice] & OPL2_TLMask); // amplitude

    kslTL = VolumeCache[voice] * kslTL;
    kslTL += kslTL + MAXVOLUME;   // round off to 0.5
    kslTL = OPL2_TLMinLevel - (kslTL / (2 * MAXVOLUME));

    kslTL |= KSLTLCache[voice] & OPL2_KSLMask;

    return (uint8_t)kslTL;
}

void FT_SetVolume(const uint8_t voice, const uint8_t volume)
{
    uint8_t const op_offset = (voice < SNAREDRUMCHANNEL || rolHeader.isMelodic) ? op_table[voice] + CARRIER_OP_OFFSET : drum_op_table[voice - SNAREDRUMCHANNEL];

    VolumeCache[voice] = volume;

    iRoboPlay->RP_WriteOpl1(OPL2_KSLTLBaseAddress + op_offset, FT_GetKSLTL(voice));
}

void FT_SetNote(const uint8_t voice, const uint16_t note)
{
    if(voice < BASSDRUMCHANNEL || rolHeader.isMelodic)
        FT_SetNoteMelodic(voice, note);
    else
        FT_SetNotePercussive(voice, note);
}

void FT_SetNoteMelodic(const uint8_t voice, const uint16_t note)
{
    iRoboPlay->RP_WriteOpl1(OPL2_KeyOnFreqHiBaseAddress + voice, KOnOctFNumCache[voice] & ~OPL2_KeyOnMask);
    KeyOnCache[voice] = false;

    if (note != SILENCENOTE)
    {
        FT_SetFreq(voice, note, true);
    }
}

void FT_SetNotePercussive(const uint8_t voice, const uint16_t note)
{
    uint8_t const channel_bit_mask = 1 << (4 - voice + BASSDRUMCHANNEL);

    AMVibRhythmCache &= ~channel_bit_mask;
    iRoboPlay->RP_WriteOpl1(OPL2_AmVibRhythmBaseAddress, AMVibRhythmCache);
    KeyOnCache[voice] = false;

    if (note != SILENCENOTE)
    {
        switch(voice)
        {
            case TOMTOMCHANNEL:
                FT_SetFreq(TOMTOMCHANNEL, note, false);
                FT_SetFreq(SNAREDRUMCHANNEL, note + TOMTOMTOSNARE, false);
                break;

            case BASSDRUMCHANNEL:
                FT_SetFreq(voice, note, false);
                break;
            default:
                // Does nothing
                break;
        }

        KeyOnCache[voice] = true;
        AMVibRhythmCache |= channel_bit_mask;
        iRoboPlay->RP_WriteOpl1(OPL2_AmVibRhythmBaseAddress, AMVibRhythmCache);
    }
}

void FT_ChangePitch(const uint8_t voice, const uint16_t pitchBend)
{
    int32_t const pitchBendLength = (int32_t)(pitchBend - MIDPITCH) * PitchRangeStep;

    if (OldPitchBendLength == pitchBendLength)
    {
        // optimisation ...
        FNumFreqPtrList[voice] = OldFNumFreqPtr;
        HalfToneOffset[voice] = OldHalfToneOffset;
    }
    else
    {
        int16_t const pitchStepDir = pitchBendLength / MIDPITCH;
        int16_t delta;
        if (pitchStepDir < 0)
        {
            int16_t const pitchStepDown = NRSTEPPITCH - 1 - pitchStepDir;
            OldHalfToneOffset = HalfToneOffset[voice] = -(pitchStepDown / NRSTEPPITCH);
            delta = (pitchStepDown - NRSTEPPITCH + 1) % NRSTEPPITCH;
            if (delta)
            {
                delta = NRSTEPPITCH - delta;
            }
        }
        else
        {
            OldHalfToneOffset = HalfToneOffset[voice] = pitchStepDir / NRSTEPPITCH;
            delta = pitchStepDir % NRSTEPPITCH;
        }
        OldFNumFreqPtr = FNumFreqPtrList[voice] = FNumNotes[delta];
        OldPitchBendLength = pitchBendLength;
    }
}

void FT_SetPitch(const uint8_t voice, const float variation)
{
    if ((voice < BASSDRUMCHANNEL) || rolHeader.isMelodic)
    {
        uint16_t const pitchBend = (variation == 1.0f) ? MIDPITCH : ((0x3fff >> 1) * variation);

        FT_ChangePitch(voice, pitchBend);
        FT_SetFreq(voice, NoteCache[voice], KeyOnCache[voice]);
    }
}

void FT_SetFreq(const uint8_t voice, const uint16_t note, const boolean keyOn)
{
    uint16_t const tmpNote = (note + HalfToneOffset[voice] < (MAXNOTES -1)) ? note + HalfToneOffset[voice] : MAXNOTES - 1;

    int const biased_note = (tmpNote > 0) ? tmpNote : 0;
    uint16_t const frequency = *(FNumFreqPtrList[voice] + NoteIndex[biased_note]);

    NoteCache[voice] = note;
    KeyOnCache[voice] = keyOn;

    KOnOctFNumCache[voice] = (NoteOctave[biased_note] << OPL2_BlockNumberShift) | ((frequency >> OPL2_FNumMSBShift) & OPL2_FNumMSBMask);

    iRoboPlay->RP_WriteOpl1(OPL2_FreqLoBaseAddress + voice, frequency & OPL2_FNumLSBMask);
    iRoboPlay->RP_WriteOpl1(OPL2_KeyOnFreqHiBaseAddress + voice, KOnOctFNumCache[voice] | (keyOn ? OPL2_KeyOnMask : 0x0));
}

void FT_SendOperator(const uint8_t voice, SOPL2OP* modulator, SOPL2OP* carrier)
{
    if ((voice < SNAREDRUMCHANNEL) ||  rolHeader.isMelodic)
    {
        uint8_t const op_offset = op_table[voice];

        iRoboPlay->RP_WriteOpl1(OPL2_AaMultiBaseAddress  + op_offset, modulator->ammulti);
        iRoboPlay->RP_WriteOpl1(OPL2_KSLTLBaseAddress    + op_offset, modulator->ksltl);
        iRoboPlay->RP_WriteOpl1(OPL2_ArDrBaseAddress     + op_offset, modulator->ardr);
        iRoboPlay->RP_WriteOpl1(OPL2_SlrrBaseAddress     + op_offset, modulator->slrr);
        iRoboPlay->RP_WriteOpl1(OPL2_FeedConBaseAddress  + voice    , modulator->fbc);
        iRoboPlay->RP_WriteOpl1(OPL2_WaveformBaseAddress + op_offset, modulator->waveform);

        KSLTLCache[voice] = carrier->ksltl;

        iRoboPlay->RP_WriteOpl1(OPL2_AaMultiBaseAddress  + op_offset + CARRIER_OP_OFFSET, carrier->ammulti);
        iRoboPlay->RP_WriteOpl1(OPL2_KSLTLBaseAddress    + op_offset + CARRIER_OP_OFFSET, FT_GetKSLTL(voice));
        iRoboPlay->RP_WriteOpl1(OPL2_ArDrBaseAddress     + op_offset + CARRIER_OP_OFFSET, carrier->ardr);
        iRoboPlay->RP_WriteOpl1(OPL2_SlrrBaseAddress     + op_offset + CARRIER_OP_OFFSET, carrier->slrr);
        iRoboPlay->RP_WriteOpl1(OPL2_WaveformBaseAddress + op_offset + CARRIER_OP_OFFSET, carrier->waveform);
    }
    else
    {
        uint8_t const op_offset = drum_op_table[voice - SNAREDRUMCHANNEL];

        KSLTLCache[voice] = modulator->ksltl;

        iRoboPlay->RP_WriteOpl1(OPL2_AaMultiBaseAddress  + op_offset, modulator->ammulti);
        iRoboPlay->RP_WriteOpl1(OPL2_KSLTLBaseAddress    + op_offset, FT_GetKSLTL(voice));
        iRoboPlay->RP_WriteOpl1(OPL2_ArDrBaseAddress     + op_offset, modulator->ardr);
        iRoboPlay->RP_WriteOpl1(OPL2_SlrrBaseAddress     + op_offset, modulator->slrr);
        iRoboPlay->RP_WriteOpl1(OPL2_WaveformBaseAddress + op_offset, modulator->waveform);
    }
}
