//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// MUS: AdLib MIDI Music File player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "mus.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

uint8_t operSlot[18] = {
    0, 0, 0,        /* 1 2 3 */
    1, 1, 1,        /* 4 5 6 */
    0, 0, 0,        /* 7 8 9 */
    1, 1, 1,        /* 10 11 12 */
    0, 0, 0,        /* 13 14 15 */
    1, 1, 1,        /* 16 17 18 */
};

uint8_t voiceSlot[18] = {
    0, 1, 2,
    0, 1, 2,
    3, 4, 5,
    3, 4, 5,
    6, 7, 8,
    6, 7, 8,
};

const uint8_t percMasks[5] =
    {0x10, 0x08, 0x04, 0x02, 0x01};

/* definition of the ELECTRIC-PIANO voice (opr0 & opr1) */
uint8_t pianoParamsOp0[nbLocParam] =
    { 1, 1, 3, 15, 5, 0, 1, 3, 15, 0, 0, 0, 1, 0 };
uint8_t pianoParamsOp1[nbLocParam] =
    { 0, 1, 1, 15, 7, 0, 2, 4,  0, 0, 0, 1, 0, 0 };

/* definition of default percussive voices: */
uint8_t bdOpr0[nbLocParam] =
    { 0,  0, 0, 10,  4, 0, 8, 12, 11, 0, 0, 0, 1, 0 };
uint8_t bdOpr1[nbLocParam] =
    { 0,  0, 0, 13,  4, 0, 6, 15,  0, 0, 0, 0, 1, 0 };
uint8_t sdOpr[nbLocParam] =
    { 0, 12, 0, 15, 11, 0, 8,  5,  0, 0, 0, 0, 0, 0 };
uint8_t tomOpr[nbLocParam] =
    { 0,  4, 0, 15, 11, 0, 7,  5,  0, 0, 0, 0, 0, 0 };
uint8_t cymbOpr[nbLocParam] =
    { 0,  1, 0, 15, 11, 0, 5,  5,  0, 0, 0, 0, 0, 0 };
uint8_t hhOpr[nbLocParam] =
    { 0,  1, 0, 15, 11, 0, 7,  5,  0, 0, 0, 0, 0, 0 };

uint8_t slotVoice[9][2] = {
    { 0, 3 },   /* voix 0 */
    { 1, 4 },   /* 1 */
    { 2, 5 },   /* 2 */
    { 6, 9 },   /* 3 */
    { 7, 10 },  /* 4 */
    { 8, 11 },  /* 5 */
    { 12, 15 }, /* 6 */
    { 13, 16 }, /* 7 */
    { 14, 17 }  /* 8 */
};

uint8_t slotPerc[5][2] = {
    { 12, 15 },     /* Bass Drum: slot 12 et 15 */
    { 16, 0 },      /* SD: slot 16 */
    { 14, 0 },      /* TOM: slot 14 */
    { 17, 0 },      /* TOP-CYM: slot 17 */
    { 13, 0 }       /* HH: slot 13 */
};

uint8_t offsetSlot[18] = {
    0,  1,  2,  3,  4,  5,
    8,  9, 10, 11, 12, 13,
    16, 17, 18, 19, 20, 21
};

boolean RP_Load(char* fileName)
{  
    uint16_t bytesRead, dataSize, readSize;
    uint16_t pageLeft = SEGMENT_SIZE;
    uint8_t* destination, i;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;
    iRoboPlay->RP_Open(fileName);
    iRoboPlay->RP_Read(&musHeader, sizeof(MUS_HEADER));

    if(musHeader.majorVersion != 1 || musHeader.minorVersion != 0 ||
        musHeader.tuneId != 0 || musHeader.tickBeat == 0 ||
        musHeader.beatMeasure == 0 || musHeader.totalTick == 0 ||
        musHeader.dataSize == 0 || musHeader.nrCommand == 0)
    {
        iRoboPlay->RP_Close();
        return false;
    }

    dataSize = musHeader.dataSize;
    currentSegment = 0;
    segmentList[currentSegment] = iRoboPlay->RP_AllocateSegment();
    iRoboPlay->RP_SetSegment(segmentList[currentSegment++]);

    destination = (uint8_t*)SEGMENT_BASE;
    while(dataSize > 0)
    {
        readSize = (dataSize < READ_BUFFER_SIZE) ? dataSize : READ_BUFFER_SIZE;
        readSize = (readSize < pageLeft) ? readSize : pageLeft;

        // It's not possible to read directly to non-primary mapper memory segments,
        // so use a buffer inbetween.
        bytesRead = iRoboPlay->RP_Read((void*)READ_BUFFER, readSize);
        memcpy(destination, (void*)READ_BUFFER, READ_BUFFER_SIZE);

        dataSize -= bytesRead;
        destination += bytesRead;
        pageLeft -= bytesRead;
        if(pageLeft == 0)
        {
            segmentList[++currentSegment] = iRoboPlay->RP_AllocateSegment();
            iRoboPlay->RP_SetSegment(segmentList[++currentSegment]);

            pageLeft = SEGMENT_SIZE;
            destination = (uint8_t *)SEGMENT_BASE;
        }
    }
    bankdata = destination;

    bytesRead = iRoboPlay->RP_Read((void*)INSTRUMENTS, INSTRUMENTS_SIZE);
    iRoboPlay->RP_Close();

    fileName[strlen(fileName)-3] = 'B';
    fileName[strlen(fileName)-2] = 'N';
    fileName[strlen(fileName)-1] = 'K';
    if(iRoboPlay->RP_Exists(fileName))
    {
        iRoboPlay->RP_Open(fileName);
    }
    else
    {
        iRoboPlay->RP_Open("STANDARD.BNK");
    }

    instSegment = ++currentSegment;
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
    bnkHeader = (BNK_HEADER*)(SEGMENT_BASE);

    iRoboPlay->RP_Close();    

    FT_FindInstruments();

    return true;
}

boolean RP_Update()
{
    if(!Counter)
    {
        Ticks = FT_GetTicks();
    }
    if(++Counter >= Ticks)
    {
        Counter = 0;
        while(Pos < musHeader.dataSize)
        {
            FT_ExecuteCommand();
            if(Pos >= musHeader.dataSize)
            {
                Pos = 0;
                return false;
            }
            else if(!FT_GetData())
            {
                Pos++;
            }
            else break;
        }
    }

    return true;
}

void RP_Rewind(int8_t subsong)
{
    uint8_t i = 0;

    // No subsongs in this format
    subsong;

    FT_SetTempo(musHeader.basicTempo, musHeader.tickBeat);

    currentSegment = 0;
    iRoboPlay->RP_SetSegment(segmentList[currentSegment]);
    songdata = (uint8_t*)SEGMENT_BASE;

    for(i = 0; i < MAX_VOICES; i++)
        Volume[i] = 0;

    Counter = 0;
    Ticks = 0;

    FT_SetMode();
    PitchRangeStep = musHeader.pitchBRange * NR_STEP_PITCH;
}

float RP_GetRefresh()
{
    uint16_t t;

    t = (musHeader.basicTempo / 60) * musHeader.tickBeat;
    return t;
}

uint8_t RP_GetSubSongs()
{
    return 0;
}

char* RP_GetPlayerInfo()
{
    return "AdLib MIDI Music File (AMF /IMP) player by RoboSoft Inc.";
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

void FT_FindInstruments()
{
    uint8_t  i, seg;
    uint8_t* instruments = (uint8_t*)INSTRUMENTS;
    uint16_t numInstruments = *(instruments + 2) + (*(instruments + 3) << 8);
    boolean  found;

    IMS_INSTRUMENT* imsInst = (IMS_INSTRUMENT*)(instruments + 4);
    for(i = 0; i < numInstruments; i++)
    {
        BNK_INSTRUMENT_HEADER* bnkInstHeader;

        seg = instSegment;
        iRoboPlay->RP_SetSegment(segmentList[seg]);

        bnkInstHeader = (BNK_INSTRUMENT_HEADER*)(SEGMENT_BASE + bnkHeader->offsetName);

        found = false;
        while(!found)
        {
            found = !strcmp(imsInst[i].name, bnkInstHeader->name);

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
                inst[i].segment = instSegment;
                while(t > SEGMENT_SIZE)
                {
                    t -= SEGMENT_SIZE;
                    inst[i].segment++;
                }
                inst[i].bnkInst = (BNK_INSTRUMENT*)(SEGMENT_BASE + t);
                break;
            }
        }
    }
}

uint8_t FT_GetData()
{
    uint8_t result = *songdata;

    Pos++;
    songdata++;
    if((uint16_t)songdata > (SEGMENT_BASE + SEGMENT_SIZE))
    {
        songdata = (uint8_t*)SEGMENT_BASE;
        iRoboPlay->RP_SetSegment(segmentList[++currentSegment]);
    }

    return result;
}

void FT_SetTempo(uint16_t tempo, uint8_t tickBeat)
{
    if(!tempo) tempo = musHeader.basicTempo;
    Timer = tempo * tickBeat / 60.0;
}

uint32_t FT_GetTicks()
{
    uint32_t ticks = 0;
    while(FT_GetData() == OVERFLOW_BYTE && Pos < musHeader.dataSize)
    {
        ticks += OVERFLOW_TICKS;
    }
    if(Pos < musHeader.dataSize)
        ticks += FT_GetData();

    if(ticks / Timer > MAX_SEC_DELAY)
        ticks = (uint32_t)(Timer * MAX_SEC_DELAY);

    return ticks;
}

void FT_ExecuteCommand()
{
    uint8_t new_status = 0, voice, haut, vol, timbre, data;
    uint16_t pitch;

    // execute MIDI command
    if (*songdata < NOTE_OFF_BYTE)
    {
        // running status
        new_status = Status;
    }
    else
        new_status = FT_GetData();
    if (new_status == STOP_BYTE)
    {
        Pos = musHeader.dataSize;
    }
    else if (new_status == SYSTEM_XOR_BYTE)
    {        
        /*
        non-standard... this is a tempo multiplier:
        data format: <F0> <7F> <00> <integer> <frac> <F7>
        tempo = basicTempo * integerPart + basicTempo * fractionPart/128
        */
        
//        if (data[pos++] != ADLIB_CTRL_BYTE ||
//            data[pos++] != TEMPO_CTRL_BYTE)
//        {
            /* unknown format ... skip all the XOR message */
//            pos -= 2;
            while (FT_GetData() != EOX_BYTE);
//        }
//        else
//        {
//            uint8_t integer = FT_GetData();
//            uint8_t frac =  FT_GetData();
//            uint16_t tempo = musHeader.basicTempo;
//            tempo = tempo * integer + ((tempo * frac) >> 7);
//            SetTempo(tempo, tickBeat);
//            FT_GetData();       /* skip EOX_BYTE */
//        }
    }
    else
    {
        Status = new_status;
        voice = Status & 0xF;
        switch (Status & 0xF0)
        {
        case NOTE_ON_BYTE:
            haut = FT_GetData();
            vol =  FT_GetData();
            if (voice > MAX_VOICES - 1)
                break;
            if (!vol)
            {
                FT_NoteOff(voice);
            }
            else
            {
                if (vol != Volume[voice])
                {
                    FT_SetVoiceVolume(voice, vol);
                    Volume[voice] = vol;
                }
                FT_NoteOn(voice, haut);
            }
            break;
        case NOTE_OFF_BYTE:
            haut = FT_GetData();
            vol = FT_GetData();
            if (voice > MAX_VOICES - 1)
                break;
            FT_NoteOff(voice);
            if (vol)
            {
                if (vol != Volume[voice])
                {
                    FT_SetVoiceVolume(voice, vol);
                    Volume[voice] = vol;
                }
                FT_NoteOn(voice, haut);
            }
            break;
        case AFTER_TOUCH_BYTE:
            vol = FT_GetData();
            if (voice > MAX_VOICES - 1)
                break;
            if (vol != Volume[voice])
            {
                FT_SetVoiceVolume(voice, vol);
                Volume[voice] = vol;
            }
            break;
        case PROG_CHANGE_BYTE:
            timbre = FT_GetData();
            if (voice > MAX_VOICES - 1)
                break;

            FT_SetVoiceTimbre(voice, timbre);
            break;
        case PITCH_BEND_BYTE:
            pitch = FT_GetData();
            pitch |= FT_GetData() << 7;
            if (voice > MAX_VOICES - 1)
                break;
            FT_SetVoicePitch(voice, pitch);
            break;
        case CONTROL_CHANGE_BYTE:
            /* unused */
            FT_GetData();
            FT_GetData();
            break;
        case CHANNEL_PRESSURE_BYTE:
            /* unused */
            FT_GetData();
            break;
        default:
            /*
            A bad status byte ( or unimplemented MIDI command) has been encontered.
            Skip bytes until next timing byte followed by status byte.
            */
            while (FT_GetData() < NOTE_OFF_BYTE && Pos < musHeader.dataSize);
            if (Pos >= musHeader.dataSize)
                break;
//            if (data[Pos] != OVERFLOW_BYTE)
//                pos--;
            break;
        }
    }
}

void FT_SetMode()
{
    if (musHeader.soundMode)
    {
        FT_SoundChut(BD);
        FT_SoundChut(SD);
        FT_SoundChut(TOM);

        /* set the frequency for the last 4 percussion voices: */
        FT_SetFreq(TOM, TOM_PITCH, 0);
        FT_SetFreq(SD, SD_PITCH, 0);
    }
    percBits = 0;

    FT_InitSlotParams();
    FT_SndSAmVibRhythm();
}

void FT_SetSlotParam(uint8_t slot, int16_t* param, uint8_t waveSel)
{
    uint8_t i;
    for (i = 0; i < nbLocParam - 1; i++)
        paramSlot[slot][i] = *param++;
    paramSlot[slot][nbLocParam - 1] = waveSel &= 0x3;
    FT_SndSetAllPrm(slot);
}

void FT_SetCharSlotParam(uint8_t slot, uint8_t* cParam, uint8_t waveSel)
{
    uint8_t i;
    int16_t param[nbLocParam];

    for (i = 0; i < nbLocParam - 1; i++)
        param[i] = *cParam++;
    FT_SetSlotParam(slot, param, waveSel);
}


void FT_InitSlotParams()
{
    uint8_t i;
    for (i = 0; i < 18; i++)
        if (operSlot[i])
            FT_SetCharSlotParam(i, pianoParamsOp1, 0);
        else
            FT_SetCharSlotParam(i, pianoParamsOp0, 0);

    if (musHeader.soundMode)
    {
        FT_SetCharSlotParam(12, bdOpr0, 0);
        FT_SetCharSlotParam(15, bdOpr1, 0);
        FT_SetCharSlotParam(16, sdOpr, 0);
        FT_SetCharSlotParam(14, tomOpr, 0);
        FT_SetCharSlotParam(17, cymbOpr, 0);
        FT_SetCharSlotParam(13, hhOpr, 0);
    }
}

void FT_SetFreq(uint8_t voice, int pitch, uint8_t keyOn)
{
    uint16_t fNbr, t1;

    voiceKeyOn[voice] = keyOn;
    notePitch[voice] = pitch;
    pitch += halfToneOffset[voice];
    if (pitch > 95)
        pitch = 95;
    if (pitch < 0)
        pitch = 0;
    fNbr = *(fNumFreqPtr[voice] + noteMOD12[pitch]);
    iRoboPlay->RP_WriteOpl1(0xA0 + voice, fNbr & 0xFF);
    t1 = keyOn ? 32 : 0;
    t1 += (noteDIV12[pitch] << 2) + (0x3 & (fNbr >> 8));
    iRoboPlay->RP_WriteOpl1(0xB0 + voice, t1);
}

void FT_SoundChut(uint8_t voice)
{
    iRoboPlay->RP_WriteOpl1(0xA0 + voice, 0);
    iRoboPlay->RP_WriteOpl1(0xB0 + voice, 0);
}

void FT_NoteOn(uint8_t voice, int16_t pitch)
{
    pitch -= (MID_C - CHIP_MID_C);
    if (pitch < 0)
        pitch = 0;
    if (pitch > 127)
        pitch = 127;

    if (voice < BD || !musHeader.soundMode)
        /* this is a melodic voice */
        FT_SetFreq(voice, pitch, 1);
    else {
        /* this is a percussive voice */
        if (voice == BD)
            FT_SetFreq(BD, pitch, 0);
        else if (voice == TOM) {
            /* for the last 4 percussions, only the TOM may change in frequency,
            modifying the three others: */
            FT_SetFreq(TOM, pitch, 0);
            FT_SetFreq(SD, pitch + TOM_TO_SD, 0);  /* f7 = 3 * f8 */
        }

        percBits |= percMasks[voice - BD];
        FT_SndSAmVibRhythm();
    }
}

void FT_NoteOff(uint8_t voice)
{
    if (!musHeader.soundMode || voice < BD)
        FT_SetFreq(voice, notePitch[voice], 0); /* shut off */
    else {
        percBits &= ~percMasks[voice - BD];
        FT_SndSAmVibRhythm();
    }
}

void FT_SetVoiceVolume(uint8_t voice, uint8_t volume)
{
    uint8_t slot;

    if (!musHeader.soundMode || voice < BD)
        slot = slotVoice[voice][1];
    else
        slot = slotPerc[voice - BD][voice == BD ? 1 : 0];

    if (volume > MAX_VOLUME)
        volume = MAX_VOLUME;
    slotRelVolume[slot] = volume;
    FT_SndSKslLevel(slot);
}

void FT_SetVoiceTimbre(uint8_t voice, uint8_t inst)
{
    int16_t wave0, wave1;
    int16_t * prm1, * wavePtr;

    wavePtr = paramArray + 2 * (nbLocParam - 1);
    wave0 = *wavePtr++;
    wave1 = *wavePtr;
    prm1 = paramArray + nbLocParam - 1;

    if (!musHeader.soundMode || voice < BD) {    /* melodic only */
        FT_SetSlotParam(slotVoice[voice][0], paramArray, wave0);
        FT_SetSlotParam(slotVoice[voice][1], prm1, wave1);
    }
    else if (voice == BD) { /* Bass Drum */
        FT_SetSlotParam(slotPerc[0][0], paramArray, wave0);
        FT_SetSlotParam(slotPerc[0][1], prm1, wave1);
    }
    else    /* percussion, 1 slot */
        FT_SetSlotParam(slotPerc[voice - BD][0], paramArray, wave0);

}

void FT_SetVoicePitch(uint8_t voice, uint8_t pitchBend)
{
    if (!musHeader.soundMode || voice <= BD) 
    {
        /* melodic + bass-drum */
        if (pitchBend > MAX_PITCH)
            pitchBend = MAX_PITCH;
        FT_ChangePitch(voice, pitchBend);
        FT_SetFreq(voice, notePitch[voice], voiceKeyOn[voice]);
    }
}

void FT_SndSAmVibRhythm()
{
    uint8_t t1;

    t1 = amDepth ? 0x80 : 0;
    t1 |= vibDepth ? 0x40 : 0;
    t1 |= musHeader.soundMode ? 0x20 : 0;
    t1 |= percBits;
    iRoboPlay->RP_WriteOpl1(0xBD, t1);
}

void FT_SndSKslLevel(uint8_t slot)
{
    uint8_t t1;

    t1 = 63 - ((uint8_t)paramSlot[slot][prmLevel] & 0x3f);   /* amplitude */
    t1 = slotRelVolume[slot] * t1;
    t1 += t1 + MAX_VOLUME;  /* round off to 0.5 */
    t1 = 63 - t1 / (2 * MAX_VOLUME);

    t1 |= (uint8_t)paramSlot[slot][prmKsl] << 6;
    iRoboPlay->RP_WriteOpl1(0x40 + offsetSlot[slot], t1 & 0xFF);
}

void FT_SndSetAllPrm(uint8_t slot)
{

}
