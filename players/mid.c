//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// Standard MIDI file player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/Header/msx_fusion.h"
#include "../fusion-c/Header/newTypes.h"

#include "../player.h"
#include "mid.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

#define BSWAPL(x) ((((uint32_t)(x) & 0x000000FFul) << 24) | (((uint32_t)(x) & 0x0000FF00ul) << 8) | (((uint32_t)(x) & 0x00FF0000ul) >> 8) | (((uint32_t)(x) & 0xFF000000ul) >> 24))

boolean RP_Load(char* fileName)
{
    uint8_t* fileData = (uint8_t*)READ_BUFFER;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;

    iRoboPlay->RP_Open(fileName);
    iRoboPlay->RP_Read(fileData, MIDI_HEADER_SIZE);

    if(fileData[0] != 'M' || fileData[1] != 'T' && fileData[2] != 'h' && fileData[3] != 'd' ||
       fileData[4] != 0 || fileData[5] != 0 || fileData[6] != 0 || fileData[7] != 6 )
    {
        return false;
    }

    if(fileData[8] != 0) return false;
    MidiHeader.format = fileData[9];
    
    if(MidiHeader.format > 2) return false;

    MidiHeader.ntracks = fileData[10];
    MidiHeader.ntracks <<= 8;
    MidiHeader.ntracks |= fileData[11];

    MidiHeader.tickdiv = fileData[12];
    MidiHeader.tickdiv <<= 8;
    MidiHeader.tickdiv |= fileData[13];

    if(MidiHeader.tickdiv < 1) return false;

    FT_LoadMIDITrackData();

    iRoboPlay->RP_Close();

    return true;  
}

boolean RP_Update()
{
    uint8_t i;

#ifdef __DEBUG
//    iRoboPlay->RP_DebugString("\n\rRP_Update: ");
#endif

    for(i = 0; i < MidiHeader.ntracks; i++)
    {
        if(!TrackList[i].trackFinished)
        {
            if(TrackList[i].deltaTime >= USEC_INC)
                TrackList[i].deltaTime -= USEC_INC;

            iRoboPlay->RP_SetSegment(TrackList[i].currentSegment);
            while(TrackList[i].deltaTime < USEC_INC && !TrackList[i].trackFinished)
            {
                // Handle current MIDI track event
                FT_HandleTrackEvent(i);

                if(TrackList[i].index >= TrackList[i].length)
                    TrackList[i].trackFinished = true;
                else
                {
                    // Fetch the next delta time
                    FT_GetVariableLen(i, &TrackList[i].deltaTime);
                    TrackList[i].deltaTime = TrackList[i].deltaTime * Tempo / MidiHeader.tickdiv;
                }
            }
        }

#ifdef __DEBUG
//        iRoboPlay->RP_DebugString("\n\r");
#endif
    }

    return true;
}

void RP_Rewind(int8_t subsong)
{
    uint8_t i;

    subsong;

    Tempo = 500000;

    for(i = 0; i < MidiHeader.ntracks; i++)
    {
        TrackList[i].currentSegment = TrackList[i].startSegment;
        TrackList[i].currentData = TrackList[i].startData;
        TrackList[i].index = 0;
        TrackList[i].trackFinished = false;

        FT_GetVariableLen(i, &TrackList[i].deltaTime);
        TrackList[i].deltaTime = TrackList[i].deltaTime * Tempo / MidiHeader.tickdiv;
    }

    FT_OplRegWrite(0x01, 0x20);
    FT_OplRegWrite(0x08, 0x40);
    FT_OplRegWrite(0xBD, 0x00);

    for(i = 0; i < VOICES_COUNT; i++)
    {
        FT_OplRegWrite(0x20 + Op1Offsets[i], 0x1);
        FT_OplRegWrite(0x20 + Op2Offsets[i], 0x1);
        FT_OplRegWrite(0x40 + Op1Offsets[i], 0x10);
        FT_OplRegWrite(0x40 + Op2Offsets[i], 0x10);

        FT_OplNoteOff(i);

        OplMem.voices2notes[i].channel = -1;
        OplMem.voices2notes[i].note = -1;
        OplMem.voices2notes[i].timbreid = -1;
    }

    for(i = 0; i < 16; i++)
    {
        uint8_t j;
        for(j = 0; j < 128; j++) OplMem.notes2voices[i][j] = -1;

        FT_ProgramChange(i, i);
        OplMem.channelvol[i] = 127;
    }
}

float RP_GetRefresh()
{
    // Fixed replay rate of 1165Hz
    return 1165.0;
}

uint8_t RP_GetSubSongs()
{
    return 0;
}

char* RP_GetPlayerInfo()
{
    return "Standard MIDI file player by RoboSoft Inc.";
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

boolean FT_LoadMIDITrackData()
{
    uint8_t i;

    uint8_t trackBuffer[4];

    uint32_t dataSize = 0;
    uint16_t readSize = 0;
    uint16_t pageLeft  = SEGMENT_SIZE;
    uint16_t bytesRead = 0;

    uint8_t* destination = (uint8_t *)SEGMENT_BASE;
    uint8_t  currentSegment = iRoboPlay->RP_AllocateSegment();

    iRoboPlay->RP_SetSegment(currentSegment);
    for(i = 0; i < MidiHeader.ntracks; i++)
    {
        TrackList[i].startSegment = currentSegment;
        TrackList[i].startData = destination;

        iRoboPlay->RP_Read(trackBuffer, 4);  // MTrk
        iRoboPlay->RP_Read(trackBuffer, 4);  // Length

        TrackList[i].length = trackBuffer[0];
        TrackList[i].length <<= 8;
        TrackList[i].length |= trackBuffer[1];
        TrackList[i].length <<= 8;
        TrackList[i].length |= trackBuffer[2];
        TrackList[i].length <<= 8;
        TrackList[i].length |= trackBuffer[3];

        dataSize = TrackList[i].length;

        while(dataSize > 0)
        {
            readSize = (dataSize < READ_BUFFER_SIZE) ? dataSize : READ_BUFFER_SIZE;
            readSize = (readSize < pageLeft) ? readSize : pageLeft;

            // It's not possible to read directly to non-primary mapper memory segments,
            // so use a buffer inbetween.
            bytesRead = iRoboPlay->RP_Read((void*)READ_BUFFER, readSize);
            memcpy(destination, (void*)READ_BUFFER, bytesRead);

            dataSize -= bytesRead;
            destination += bytesRead;
            pageLeft -= bytesRead;
            if(pageLeft == 0)
            {
                currentSegment = iRoboPlay->RP_AllocateSegment();
                iRoboPlay->RP_SetSegment(currentSegment);

                pageLeft = SEGMENT_SIZE;
                destination = (uint8_t *)SEGMENT_BASE;
            }
        }
    }

    return true;
}

uint8_t FT_GetTrackData(uint8_t track)
{   
    uint8_t data = *(TrackList[track].currentData++);

    TrackList[track].index++;

    if(TrackList[track].currentData == (uint8_t *)(SEGMENT_BASE + SEGMENT_SIZE))
    {
        TrackList[track].currentSegment++;
        iRoboPlay->RP_SetSegment(TrackList[track].currentSegment);
        TrackList[track].currentData = (uint8_t*)SEGMENT_BASE;
    }

    return data;
}

void FT_GetVariableLen(uint8_t track, uint32_t* value)
{
    uint8_t byteBuf;

    *value = 0;

    for(;;)
    {
        byteBuf = FT_GetTrackData(track);
        *value <<= 7;
        *value |= (byteBuf & 127);
        if((byteBuf & 128) == 0) break;
    }
}

void FT_HandleTrackEvent(uint8_t track)
{
    uint8_t dataByte1, dataByte2, dataByte3;

    dataByte1 =  FT_GetTrackData(track);

    // If MSB is not set, this is a running status
    if((dataByte1 & 128) != 0)
    {
        TrackList[track].statusByte = dataByte1;
        dataByte2 = FT_GetTrackData(track);
    }
    else
    {
        dataByte2 = dataByte1;
        dataByte1 = TrackList[track].statusByte;
    }

    if(TrackList[track].statusByte == 0xFF)
        FT_HandleMetaEvent(track, dataByte2);
    else if((TrackList[track].statusByte >= 0xF0) && (TrackList[track].statusByte <= 0xF7))
        FT_HandleSysExEvent(track, dataByte2);
    else
    {
        switch(TrackList[track].statusByte & 0xF0)
        {
            case 0x80:  // Note OFF
                dataByte3 = FT_GetTrackData(track);
                FT_NoteOff(dataByte1 & 0x0F, dataByte2 & 127);
                break;
            case 0x90:  // Note ON
                dataByte3 = FT_GetTrackData(track);
                FT_NoteOn(dataByte1 & 0x0F, dataByte2 & 127, dataByte3);
                break;
            case 0xA0:  // Key after-touch
                dataByte3 = FT_GetTrackData(track);
                break;
            case 0xB0:  // Control change
                dataByte3 = FT_GetTrackData(track);
                FT_ControlChange(dataByte1 & 0x0F, dataByte2, dataByte3);
                break;
            case 0xC0:  // Program change
                FT_ProgramChange(dataByte1 & 0x0F, dataByte2 & 127);
                break;
            case 0xD0:  // Channel after-touch
                dataByte2 = FT_GetTrackData(track);               
                break;
            case 0xE0:  // Pitch wheel
                dataByte3 = FT_GetTrackData(track);
                FT_PitchWheel(dataByte1 & 0x0F, ((dataByte3 <<= 7) | dataByte2));
                break;
        }
    }
}

void FT_HandleMetaEvent(uint8_t track, uint8_t subType)
{
    uint32_t metaLen, i;

    FT_GetVariableLen(track, &metaLen);

    for(i = 0; i < metaLen; i++)
    {
        FT_GetTrackData(track);        
    }
}

void FT_HandleSysExEvent(uint8_t track, uint8_t subType)
{
    uint32_t sysexLen, i;

    FT_GetVariableLen(track, &sysexLen);

    for(i = 0; i < sysexLen; i++)
    {
        FT_GetTrackData(track);        
    }
}

void FT_OplRegWrite(uint16_t reg, uint8_t data)
{
    if((reg & 0x100) != 0 )
        iRoboPlay->RP_WriteOpl2(reg & 0xFF, data);
    else
        iRoboPlay->RP_WriteOpl1(reg & 0xFF, data);
}

void FT_LoadInstrument(int8_t voice, TIMBRE_T* timbre)
{
  /* KSL (key level scaling) / attenuation */
  FT_OplRegWrite(0x40 + Op1Offsets[voice], timbre->modulator_40);
  FT_OplRegWrite(0x40 + Op2Offsets[voice], timbre->carrier_40 | 0x3f); /* force volume to 0, it will be reajusted during 'note on' */

  /* select waveform on both operators */
  FT_OplRegWrite(0xE0 + Op1Offsets[voice], timbre->modulator_E862 >> 24);
  FT_OplRegWrite(0xE0 + Op2Offsets[voice], timbre->carrier_E862 >> 24);

  /* sustain / release */
  FT_OplRegWrite(0x80 + Op1Offsets[voice], (timbre->modulator_E862 >> 16) & 0xff);
  FT_OplRegWrite(0x80 + Op2Offsets[voice], (timbre->carrier_E862 >> 16) & 0xff);

  /* attack rate / decay */
  FT_OplRegWrite(0x60 + Op1Offsets[voice], (timbre->modulator_E862 >> 8) & 0xff);
  FT_OplRegWrite(0x60 + Op2Offsets[voice], (timbre->carrier_E862 >> 8) & 0xff);

  /* AM / vibrato / envelope */
  FT_OplRegWrite(0x20 + Op1Offsets[voice], timbre->modulator_E862 & 0xff);
  FT_OplRegWrite(0x20 + Op2Offsets[voice], timbre->carrier_E862 & 0xff);

  /* feedback / connection */
  if (voice >= 9) {
    voice -= 9;
    voice |= 0x100;
  }
  /* On OPL3 make sure to enable LEFT/RIGHT unmute bits */
  FT_OplRegWrite(0xC0 + voice, timbre->feedconn | 0x30);
}

void FT_VoiceVolume(int8_t voice, uint8_t program, int16_t volume)
{
    uint8_t carrierVal = GMtimbres[program].carrier_40;
    if(volume == 0) carrierVal |= 0x3F;
    else FT_CalcVolume(&carrierVal, volume);

    FT_OplRegWrite(0x40 + Op2Offsets[voice], carrierVal);
}

void FT_CalcVolume(uint8_t* regbyte, int16_t volume)
{
    int16_t level;

    // Invert bits and strip out the KSL header
    level = ~(*regbyte);
    level &= 0x3f;

    // Adjust volume
    level = (level * volume) / 127;

    // Boundaries check
    if(level > 0x3F) level = 0x3F;
    if(level < 0) level = 0;

    // Invert the bits, as expected by the OPL registers
    level = ~level;
    level &= 0x3F;

    // Calculate final result
    *regbyte &= 0xC0;   // Zero out all attenuation bits
    *regbyte |= level;
}

void FT_OplNoteOn(int8_t voice, uint8_t note, int8_t pitch)
{
    uint16_t freq = FreqTable[note];
    uint8_t octave = OctaveTable[note];

    if(pitch != 0)
    {
        freq = ((uint32_t)freq * PitchTable[pitch + 128]) >> 15;
        if(freq >= 1024)
        {
            freq >>= 1;
            octave++;
        }
    }
    if(octave > 7) octave = 7;

    if(voice >= 9)
    {
        voice -= 9;
        voice |= 0x100;
    }

    FT_OplRegWrite(0xA0 + voice, freq & 0xFF);                      // Set lowfreq
    FT_OplRegWrite(0xB0 + voice, (freq >> 8) | (octave << 2) | 32); // KEY ON + hifreq + octave
}

void FT_OplNoteOff(int8_t voice)
{
    if(voice >= 9) FT_OplRegWrite(0x1B0 + voice - 9, 0);
    else FT_OplRegWrite(0xB0 + voice, 0);
}

void FT_NoteOn(int8_t channel, uint8_t note, uint8_t velocity)
{
    int8_t x, voice = -1;
    uint8_t lowestPriorityVoice = 0;
    uint8_t instrument;

    // Get the instrument to play
    if(channel == 9) instrument = 128 | note;
    else instrument = OplMem.channelprog[channel];

    // If note already playing then reuse its voice
    if(OplMem.notes2voices[channel][note] >= 0)   
        voice = OplMem.notes2voices[channel][note];
    else
    {
        // Find a free voice, possibly with the right timbre, or at least locate the oldest note
        for(x = 0; x < VOICES_COUNT; x++)
        {
            if(OplMem.voices2notes[x].channel < 0)
            {
                voice = x; // Preselect this voice but continue looking
                // If the instrument is right, do not look further
                if(OplMem.voices2notes[x].timbreid == instrument) break;
            }
            if(OplMem.voices2notes[x].priority < OplMem.voices2notes[lowestPriorityVoice].priority)
                lowestPriorityVoice = x;
        }
        // If no free voice available then abort the oldest one
        if(voice < 0)
        {
            voice = lowestPriorityVoice;
            FT_NoteOff(OplMem.voices2notes[voice].channel, OplMem.voices2notes[voice].note);
        }
    }

    // Load the proper instrument if not already good
    if(OplMem.voices2notes[voice].timbreid != instrument)
    {
        OplMem.voices2notes[voice].timbreid = instrument;
        FT_LoadInstrument(voice, &(GMtimbres[instrument]));
    }

    // Update state
    OplMem.voices2notes[voice].channel = channel;
    OplMem.voices2notes[voice].note = note;
    OplMem.voices2notes[voice].priority = ((16 - channel) << 8) | 0xFF;
    OplMem.notes2voices[channel][note] = voice;

    // Set the requested velocity on the voice
    FT_VoiceVolume(voice, OplMem.voices2notes[voice].timbreid, velocity * OplMem.channelvol[channel] / 127);

    // Trigger NOTE ON on the OPL
    if(channel == 9)
        FT_OplNoteOn(voice, 60, OplMem.channelpitch[channel]);
    else
        FT_OplNoteOn(voice, note, OplMem.channelpitch[channel]);

    // Readjust all priorities
    for(x = 0; x < VOICES_COUNT; x++)
    {
        if(OplMem.voices2notes[x].priority > 0) OplMem.voices2notes[x].priority -= 1;
    }
}

void FT_NoteOff(int8_t channel, uint8_t note)
{
    int8_t voice = OplMem.notes2voices[channel][note];

    if(voice >= 0)
    {
        FT_OplNoteOff(voice);

        OplMem.voices2notes[voice].channel = -1;
        OplMem.voices2notes[voice].note = -1;
        OplMem.voices2notes[voice].priority = -1;
        OplMem.notes2voices[channel][note] = -1;
    }
}

void FT_ControlChange(int8_t channel, uint8_t id, uint8_t value)
{
    uint8_t x;

    switch(id)
    {
        case 11:    // "Expression" (meaning "channel volume")
            OplMem.channelvol[channel] = value;
            break;
        case 123:   // 'all notes off'
        case 120:   // 'all sound off'
            for(x = 0; x < VOICES_COUNT; x++)
            {
                if(OplMem.voices2notes[x].channel != channel) continue;
                FT_NoteOff(channel, OplMem.voices2notes[x].note);
            }
            break;
    }
}

void FT_ProgramChange(int8_t channel, uint8_t program)
{
    if(channel == 9) return;
    OplMem.channelprog[channel] = program;
}

void FT_PitchWheel(int8_t channel, uint16_t wheel)
{
    OplMem.channelpitch[channel] = wheel;
}
