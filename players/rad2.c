//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// RAD: Reality ADLib Tracker v2.1 player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/Header/msx_fusion.h"
#include "../fusion-c/Header/newTypes.h"

#include "../player.h"
#include "rad2.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

boolean RP_Load(char* fileName)
{  
    uint8_t   b, i, j;
    uint8_t   inst_num, alg, track_num, riff_id, riff_num, chan_num;
    uint8_t*  op;
    uint16_t  size, b1, b2;
    RAD_INST* inst;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;

    FileData = (uint8_t*)SEGMENT_BASE;

    iRoboPlay->RP_Open(fileName);
    iRoboPlay->RP_Read(FileData, SEGMENT_SIZE);
    iRoboPlay->RP_Close();

    Header = (RAD_HEADER*)FileData;

    if(strncmp(Header->id,"RAD by REALiTY!!",16) || Header->version != 0x21) return false;

    for(i = 0; i < RAD_MAX_TRACK; i++) Tracks[i] = 0;

    for(i = 0; i < RAD_MAX_RIFF_TRACK; i++)
        for(j = 0; j < RAD_MAX_CHANNEL; j++)
            Riffs[i][j] = 0;

    FileData = (uint8_t *)SEGMENT_BASE + sizeof(RAD_HEADER);

    Speed = Header->flags & 0x1F;
    
    // Is BPM value present?
    Hertz = 50;
    if(Header->flags & 0x20)
    {
        b1 = FileData[0];
        b2 = FileData[1] << 8;
        Hertz = (b1 | b2) * 2 / 5;
        FileData += 2;
    }

    // Slow timer tune?
    if(Header->flags & 0x40) Hertz = 18.0;

    // Skip any description
    while(*FileData) FileData++;
    FileData++;

    // Unpack the instruments
    while(1)
    {
        inst_num = *FileData++;
        if(inst_num == 0) break;

        FileData += *FileData++;
        inst = &Instruments[inst_num - 1];

        alg = *FileData++;
        inst->algorithm = alg & 7;
        inst->panning[0] = (alg >> 3) & 3;
        inst->panning[1] = (alg >> 5) & 3;

        if(inst->algorithm < 7)
        {
            b = *FileData++;
            inst->feedback[0] = b & 15;
            inst->feedback[1] = b >> 4;

            b = *FileData++;
            inst->detune = b >> 4;
            inst->riffSpeed = b & 15;

            inst->volume = *FileData++;

            for(i = 0; i < 4; i++)
            {
                op = inst->operators[i];
                for(j = 0; j < 5; j++)
                    op[j] = *FileData++;
            }
        }
        else
        {
            FileData += 6;
        }

        if(alg & 0x80)
        {
            b1 = FileData[0];
            b2 = FileData[1] << 8;
            size = b1 | b2;
            FileData += 2;
            inst->riff = FileData;
            FileData += size;
        }
        else inst->riff = 0;
    } 

    // Get order list
    OrderListSize = *FileData++;
    OrderList = FileData;
    FileData += OrderListSize;

    // Locate the tracks
    while(1)
    {
        track_num = *FileData++;
        if(track_num >= RAD_MAX_TRACK) break;

        b1 = FileData[0];
        b2 = FileData[1] << 8;
        size = b1 | b2;
        FileData += 2;

        Tracks[track_num] = FileData;
        FileData += size;
    }

    // Locate the riffs
    while(1)
    {
        riff_id = *FileData++;
        riff_num = riff_id >> 4;
        chan_num = riff_id & 15;
        if(riff_num >= RAD_MAX_RIFF_TRACK || chan_num > RAD_MAX_CHANNEL) break;

        b1 = FileData[0];
        b2 = FileData[1] << 8;
        size = b1 | b2 + SEGMENT_BASE;
        FileData += 2;

        Riffs[riff_num][chan_num - 1] = FileData;
        FileData += size;      
    }

    return true;
}

boolean RP_Update()
{
    uint8_t i;

    // Run riffs
    for(i = 0; i < RAD_MAX_CHANNEL; i++)
    {
        FT_TickRiff(i, &Channels[i].iRiff, false);
        FT_TickRiff(i, &Channels[i].riff, false);
    }

    // Run maintrack
    FT_PlayLine();

    // Run effects
    for(i = 0; i < RAD_MAX_CHANNEL; i++)
    {
        FT_ContinueFX(i, &Channels[i].iRiff.fx);
        FT_ContinueFX(i, &Channels[i].riff.fx);
        FT_ContinueFX(i, &Channels[i].fx);
    }

    return true;
}

void RP_Rewind(int8_t subsong)
{
    uint16_t i;

    subsong;

    for(i = 0; i < 512; i++) OPL3Regs[i] = 255;

    // Configure OPL3
    FT_SetOPL3(1, 0x20);  // Allow waveforms
    FT_SetOPL3(8, 0);     // No split point
    FT_SetOPL3(0xbd, 0);  // No drums, etc.
    FT_SetOPL3(0x104, 0); // Everything 2-op by default
    FT_SetOPL3(0x105, 1); // OPL3 mode on

    // Initialise play values
    SpeedCnt = 1;
    Order = 0;
    Track = FT_GetTrack();
    Line = 0;
    Entrances = 0;
    MasterVol = 64;

    for(i = 0; i < RAD_MAX_CHANNEL; i++)
    {
        Channels[i].lastInstrument = 0;
        Channels[i].instrument = 0;
        Channels[i].volume = 0;
        Channels[i].detuneA = 0;
        Channels[i].detuneB = 0;
        Channels[i].keyFlags = 0;
        Channels[i].riff.speedCnt = 0;
        Channels[i].iRiff.speedCnt = 0;
    }

}

float RP_GetRefresh()
{
    return Hertz;
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

uint8_t* FT_GetTrack()
{   
    uint8_t track_num = 0;
    uint16_t* patternList = (uint16_t *)Tracks;

    if(Order >= OrderListSize) Order = 0;
    track_num = OrderList[Order];

    if(track_num & 0x80)
    {
        Order = track_num & 0x7F;
        track_num = OrderList[Order] & 0x7F;
    }

    return (uint8_t *)(Tracks[track_num]);
}

void FT_PlayLine()
{
    uint8_t i;
    uint8_t lineId = 0;
    uint8_t channum = 0;
    boolean last = false;

    SpeedCnt--;
    if(SpeedCnt > 0) return;

    SpeedCnt = Speed;
    for(i = 0; i < RAD_MAX_CHANNEL; i++) FT_ResetFX(&Channels[i].fx);

    LineJump = -1;

    // At the right line?
    if(Track && (*Track & 0x7F) <= Line)
    {
        lineId = *Track++;
        do
        {
            channum = *Track & 15;
            last = FT_UnpackNote(&Channels[channum].lastInstrument);
            FT_PlayNote(channum, SNone, 0);
        } while (!last);

        // Was this the last line?
        if(lineId & 0x80) Track = 0;
    }
    
    // Move to next line
    Line++;

    if(Line > RAD_MAX_LINE/* || LineJump >= 0*/)
    {
        /*
        if (LineJump >= 0)
            Line = LineJump;
        else*/
            Line = 0;

        // Move to next track in order list
        Order++;
        Track = FT_GetTrack();       
    }
}

boolean FT_UnpackNote(uint8_t* last_instrument)
{
    uint8_t n;
    uint8_t note = 0;
    uint8_t chanid = *Track++;

    InstNum = 0;
    EffectNum = 0;
    Param = 0;

    // Unpack note data
    if(chanid & 0x40)
    {
        n = *Track++;
        note = n & 0x7F;

        // Retrigger last instrument?
        if(n & 0x80) InstNum = *last_instrument;
    }

    // Do we have an instrument?
    if(chanid & 0x20)
    {
        InstNum = *Track++;
        *last_instrument = InstNum;
    }

    // Do we have an effect?
    if(chanid & 0x10)
    {
        EffectNum = *Track++;
        Param = *Track++;
    }

    NoteNum = note & 0x0F;
    OctaveNum = note >> 4;

    return ((chanid & 0x80) != 0);
}

void FT_PlayNote(uint8_t channum, RAD_SOURCE source, uint8_t op)
{
    RAD_CHANNEL* chan = &Channels[channum];

    int8_t    val, p0, p1, which, fb;
    uint8_t   slideSpeed;
    boolean   transposing = false;

    RAD_EFFECTS* fx = &chan->fx;
    RAD_INST* oldinst;
    RAD_INST* inst;

    if(Entrances >= 8) return;
    Entrances++;

    // Select which effects source we're using
    if(source == SRiff)
        fx = &chan->riff.fx;
    else if(source == SIRiff)
        fx = &chan->iRiff.fx;

    if(EffectNum == RAD_CMD_TONE_SLIDE)
    {
        if(NoteNum > 0 && NoteNum <= 12)
        {
            fx->toneSlideOct = OctaveNum;
            fx->toneSlideFreq = NoteFreq[NoteNum - 1];
        }
        goto toneslide;
    }

    // Playing a new instrument?
    if(InstNum > 0)
    {
        oldinst = chan->instrument;
        inst = &Instruments[InstNum - 1];
        chan->instrument = inst;

        // Ignore MIDI instruments
        if(inst->algorithm == 7)
        {
            Entrances--;
            return;
        }

        FT_LoadInstrumentOPL3(channum);

        // Bounce the channel
        chan->keyFlags |= fKeyOff | fKeyOn;

        FT_ResetFX(&chan->iRiff.fx);

        if(source != SIRiff || inst != oldinst)
        {
            // Instrument riff?
           if (inst->riff && inst->riffSpeed > 0) {

                chan->iRiff.track = chan->iRiff.trackStart = inst->riff;
                chan->iRiff.line = 0;
                chan->iRiff.speed = inst->riffSpeed;
                chan->iRiff.lastInstrument = 0;

                // Note given with riff command is used to transpose the riff
                if (NoteNum >= 1 && NoteNum <= 12) 
                {
                    chan->iRiff.transposeOctave = OctaveNum;
                    chan->iRiff.transposeNote = NoteNum;
                    transposing = true;
                } else 
                {
                    chan->iRiff.transposeOctave = 3;
                    chan->iRiff.transposeNote = 12;
                }

                // Do first tick of riff
                chan->iRiff.speedCnt = 1;
                FT_TickRiff(channum, &chan->iRiff, false);

            } else
                chan->iRiff.speedCnt = 0;
        } 
    }

    // Starting a channel riff?
    if(EffectNum == RAD_CMD_RIFF || EffectNum == RAD_CMD_TRANSPOSE)
    {
        FT_ResetFX(&chan->riff.fx);

        p0 = Param / 10;
        p1 = Param % 10;
        chan->riff.track = p1 > 0 ? Riffs[p0][p1 - 1] : 0;
        if(chan->riff.track)
        {
            chan->riff.trackStart = chan->riff.track;
            chan->riff.line = 0;
            chan->riff.speed = Speed;
            chan->riff.lastInstrument = 0;

            // Note given with riff command is used to transpose the riff
            if(EffectNum == RAD_CMD_TRANSPOSE && NoteNum >= 1 && NoteNum <= 12)
            {
                chan->riff.transposeOctave = OctaveNum;
                chan->riff.transposeNote = NoteNum;
                transposing = true;
            }
            else
            {
                chan->riff.transposeOctave = 3;
                chan->riff.transposeNote = 12;               
            }

            // Do first tick of riff
            chan->riff.speedCnt = 1;
            FT_TickRiff(channum, &chan->riff, true);
        }
    }

    // Play the note
    if(!transposing && NoteNum > 0)
    {
        if(NoteNum == 15) chan->keyFlags |= fKeyOff;

        if(!chan->instrument || chan->instrument->algorithm < 7)
            FT_PlayNoteOPL3(channum);
    }

    // Process efect
    switch(EffectNum)
    {
        case RAD_CMD_SET_VOL:
            FT_SetVolume(channum, Param);
            break;
        case RAD_CMD_SET_SPEED:
            if (source == SNone) 
            {
                Speed = Param;
                SpeedCnt = Param;
            } else if (source == SRiff) 
            {
                chan->riff.speed = Param;
                chan->riff.speedCnt = Param;
            } else if (source == SIRiff) 
            {
                chan->iRiff.speed = Param;
                chan->iRiff.speedCnt = Param;
            }            
            break;
        case RAD_CMD_PORTAMENTO_UP:
            fx->portSlide = Param;
            break;
        case RAD_CMD_PORTAMENTO_DOWN:
            fx->portSlide = -(int8_t)Param;
            break;
        case RAD_CMD_TONE_VOL_SLIDE:
        case RAD_CMD_VOL_SLIDE:
            {
                val = Param;
                if(val >= 50) val = -(val - 50);
                fx->volSlide = val;
                if(EffectNum != RAD_CMD_TONE_VOL_SLIDE) break;
            }
        case RAD_CMD_TONE_SLIDE:
toneslide:
            slideSpeed = Param;
            if(slideSpeed) fx->toneSlideSpeed = slideSpeed;
            FT_GetSlideDir(channum, fx);
            break;
        case RAD_CMD_JUMP_TO_LINE:
            if(Param >= RAD_MAX_LINE) break;
            if(source == SNone)
                LineJump = Param;
            break;
        case RAD_CMD_MULTIPLIER:
            if(source == SIRiff)
                FT_LoadInstMultiplierOPL3(channum, op, Param);
            break;
        case RAD_CMD_VOLUME:
            if(source == SIRiff)
                FT_LoadInstVolumeOPL3(channum, op, Param);
            break;
        case RAD_CMD_FEEDBACK:
            if(source == SIRiff)
            {
                which = Param / 10;
                fb = Param % 10;
                FT_LoadInstFeedbackOPL3(channum, which, fb);
            }
            break;        
    }

    Entrances--;
}

void FT_SetVolume(uint8_t channel, uint8_t volume)
{
    volume = (volume <= 63) ? volume : 63;
    Channels[channel].volume = volume;

//    iRoboPlay->RP_WriteOpl1(0x40 + Op_table[channel], 63 - volume + (Channels[channel].instrument->modulator_40 & 192));
//    iRoboPlay->RP_WriteOpl1(0x43 + Op_table[channel], 63 - volume + (Channels[channel].instrument->carrier_43 & 192));
}

void FT_ResetFX(RAD_EFFECTS* fx)
{
    fx->portSlide = 0;
    fx->volSlide = 0;
    fx->toneSlideDir = 0;
}

void FT_ContinueFX(uint8_t channel, RAD_EFFECTS* fx)
{
    int8_t vol = 0;

    if(fx->portSlide)
        FT_Portamento(channel, fx, fx->portSlide, false);

    if(fx->volSlide)
    {
        vol = Channels[channel].volume;
        vol -= fx->volSlide;
        if(vol < 0) vol = 0;
        FT_SetVolume(channel, vol);
    }

    if(fx->toneSlideDir)
        FT_Portamento(channel, fx, fx->toneSlideDir, true);
}

void FT_TickRiff(uint8_t channum, RAD_RIFF* riff, boolean chan_riff)
{
    uint8_t lineid, line, col;
    uint8_t* trk;
    boolean last;

    if (riff->speedCnt == 0) 
    {
        FT_ResetFX(&riff->fx);
        return;
    }

    riff->speedCnt--;
    if (riff->speedCnt > 0)
        return;
    riff->speedCnt = riff->speed;

    line = riff->line++;
    if (riff->line >= RAD_MAX_LINE)
        riff->speedCnt = 0;

    FT_ResetFX(&riff->fx);

    // Is this the current line in track?
    trk = riff->track;
    if (trk && (*trk & 0x7F) == line) 
    {
        lineid = *trk++;

        if (chan_riff) 
        {
            // Channel riff: play current note
            FT_UnpackNote(&riff->lastInstrument);
            FT_Transpose(riff->transposeNote, riff->transposeOctave);

            FT_PlayNote(channum, SRiff, 0);

        } else 
        {

            // Instrument riff: here each track channel is an extra effect that can run, but is not
            // actually a different physical channel
            do {
                col = *trk & 15;
                last = FT_UnpackNote(&riff->lastInstrument);
                if (EffectNum != RAD_CMD_IGNORE)
                    FT_Transpose(riff->transposeNote, riff->transposeOctave);
                FT_PlayNote(channum, SIRiff, col > 0 ? (col - 1) & 3 : 0);
            } while (!last);
        }

        // Last line?
        if (lineid & 0x80)
            trk = 0;

        riff->track = trk;
    }

    // Special case; if next line has a jump command, run it now
    if (!trk || (*trk++ & 0x7F) != riff->line)
        return;

    FT_UnpackNote(&lineid); // lineid is just a dummy here
    if (EffectNum == RAD_CMD_JUMP_TO_LINE && Param < RAD_MAX_LINE) {
        riff->line = Param;
        riff->track = FT_SkipToLine(riff->trackStart, Param, chan_riff);
    }

}

uint8_t* FT_SkipToLine(uint8_t* trk, uint8_t linenum, boolean chan_riff) 
{
    uint8_t lineid, chanid;
    while (1) 
    {
        lineid = *trk;
        if ((lineid & 0x7F) >= linenum)
            return FileData;
        if (lineid & 0x80)
            break;
        trk++;

        // Skip channel notes
        do {
            chanid = *trk++;
            trk += NoteSize[(chanid >> 4) & 7];
        } while (!(chanid & 0x80) && !chan_riff);
    }

    return 0;
}

void FT_Transpose(int8_t note, int8_t octave) 
{
    uint8_t toct, tnot;
    if (NoteNum >= 1 && NoteNum <= 12) 
    {
        toct = octave - 3;
        if (toct != 0) 
        {
            OctaveNum += toct;
            if (OctaveNum < 0)
                OctaveNum = 0;
            else if (OctaveNum > 7)
                OctaveNum = 7;
        }

        tnot = note - 12;
        if (tnot != 0) {
            NoteNum += tnot;
            if (NoteNum < 1) {
                NoteNum += 12;
                if (OctaveNum > 0)
                    OctaveNum--;
                else
                    NoteNum = 1;
            }
        }
    }
}

void FT_Portamento(uint8_t channum, RAD_EFFECTS* fx, int8_t amount, boolean toneSlide)
{
    RAD_CHANNEL* chan = &Channels[channum];

    uint16_t freq = chan->currFreq, frq2, chan_offset;
    uint8_t oct = chan->currOctave;

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
            if(oct > fx->toneSlideOct || 
              (oct == fx->toneSlideOct && freq >= fx->toneSlideFreq))
            {
                freq = fx->toneSlideFreq;
                oct = fx->toneSlideOct;
            } 
        } else
        {
            if(oct < fx->toneSlideOct || 
              (oct == fx->toneSlideOct && freq <= fx->toneSlideFreq))
            {
                freq = fx->toneSlideFreq;
                oct = fx->toneSlideOct;
            } 
        }
    }

    chan->currFreq = freq;
    chan->currOctave = oct;

    // Apply detunes
    frq2 = freq - chan->detuneB;
    freq += chan->detuneA;

    // Write value back to OPL3
    chan_offset = Chn2Offsets3[channum];
    FT_SetOPL3(0xA0 + chan_offset, freq & 0xFF);
    FT_SetOPL3(0xB0 + chan_offset, (freq >> 8 & 3) | oct << 2 | (FT_GetOPL3(0xB0 + chan_offset) & 0xE0));

    chan_offset = ChanOffsets3[channum];
    FT_SetOPL3(0xA0 + chan_offset, frq2 & 0xFF);
    FT_SetOPL3(0xB0 + chan_offset, (frq2 >> 8 & 3) | oct << 2 | (FT_GetOPL3(0xB0 + chan_offset) & 0xE0));

}

void FT_GetSlideDir(uint8_t channum, RAD_EFFECTS* fx)
{
    RAD_CHANNEL* chan = &Channels[channum];

    uint8_t oct;
    uint16_t freq;

    uint8_t oldoct;
    uint16_t oldfreq;

    int8_t speed = fx->toneSlideSpeed;

    if(speed > 0)
    {
        oct = fx->toneSlideOct;
        freq = fx->toneSlideFreq;

        oldfreq = chan->currFreq;
        oldoct = chan->currOctave;

        if(oldoct > oct) speed = -speed;
        else if(oldoct == oct)
        {
            if(oldfreq > freq) speed = -speed;
            else if (oldfreq == freq) speed = 0;
        }
    }

    fx->toneSlideDir = speed;
}

void FT_SetOPL3(uint16_t reg, uint8_t val)
{
    OPL3Regs[reg] = val;
    if(reg < 0x100) 
        iRoboPlay->RP_WriteOpl1(reg, val);
    else
        iRoboPlay->RP_WriteOpl2(reg - 0x100, val);
}

uint8_t FT_GetOPL3(uint16_t reg)
{
    return OPL3Regs[reg];
}

void FT_PlayNoteOPL3(uint8_t channum)
{
    boolean op4;
    uint16_t freq, frq2;

    RAD_CHANNEL* chan = &Channels[channum];

    uint16_t o1 = ChanOffsets3[channum];
    uint16_t o2 = Chn2Offsets3[channum];

    // Key off the channel
    if (chan->keyFlags & fKeyOff) 
    {
        chan->keyFlags &= ~(fKeyOff | fKeyedOn);
        FT_SetOPL3(0xB0 + o1, FT_GetOPL3(0xB0 + o1) & ~0x20);
        FT_SetOPL3(0xB0 + o2, FT_GetOPL3(0xB0 + o2) & ~0x20);
    }

    if (NoteNum == 15)
        return;

    op4 = (chan->instrument && chan->instrument->algorithm >= 2);

    freq = NoteFreq[NoteNum - 1];
    frq2 = freq;

    chan->currFreq = freq;
    chan->currOctave = OctaveNum;

    // Detune.  We detune both channels in the opposite direction so the note retains its tuning
    freq += chan->detuneA;
    frq2 -= chan->detuneB;

    // Frequency low byte
    if (op4)
        FT_SetOPL3(0xA0 + o1, frq2 & 0xFF);
    FT_SetOPL3(0xA0 + o2, freq & 0xFF);

    // Frequency high bits + octave + key on
    if (chan->keyFlags & fKeyOn)
        chan->keyFlags = (chan->keyFlags & ~fKeyOn) | fKeyedOn;
    if (op4)
        FT_SetOPL3(0xB0 + o1, (frq2 >> 8) | (OctaveNum << 2) | ((chan->keyFlags & fKeyedOn) ? 0x20 : 0));
    else
        FT_SetOPL3(0xB0 + o1, 0);
    FT_SetOPL3(0xB0 + o2, (freq >> 8) | (OctaveNum << 2) | ((chan->keyFlags & fKeyedOn) ? 0x20 : 0));

}

void FT_LoadInstrumentOPL3(uint8_t channum)
{
    static const uint8_t blank[] = { 0, 0x3F, 0, 0xF0, 0 };
    uint8_t i, alg, mask;
    uint8_t *op;
    uint16_t reg, vol;

    RAD_CHANNEL* chan = &Channels[channum];
    RAD_INST* inst = chan->instrument;
    if (!inst)
        return;

    alg = inst->algorithm;
    chan->volume = inst->volume;
    chan->detuneA = (inst->detune + 1) >> 1;
    chan->detuneB = inst->detune >> 1;

    // Turn on 4-op mode for algorithms 2 and 3 (algorithms 4 to 6 are simulated with 2-op mode)
    if (channum < 6) 
    {
        mask = 1 << channum;
        FT_SetOPL3(0x104, (FT_GetOPL3(0x104) & ~mask) | (alg == 2 || alg == 3 ? mask : 0));
    }

    // Left/right/feedback/algorithm
    FT_SetOPL3(0xC0 + ChanOffsets3[channum], ((inst->panning[1] ^ 3) << 4) | inst->feedback[1] << 1 | (alg == 3 || alg == 5 || alg == 6 ? 1 : 0));
    FT_SetOPL3(0xC0 + Chn2Offsets3[channum], ((inst->panning[0] ^ 3) << 4) | inst->feedback[0] << 1 | (alg == 1 || alg == 6 ? 1 : 0));

    // Load the operators
    for (i = 0; i < 4; i++) 
    {     
        op = (alg < 2 && i >= 2) ? blank : inst->operators[i];
        reg = OpOffsets3[channum][i];

        vol = ~op[1] & 0x3F;

        // Do volume scaling for carriers
        if (AlgCarriers[alg][i]) {
            vol = vol * inst->volume / 64;
            vol = vol * MasterVol / 64;
        }

        FT_SetOPL3(reg + 0x20, op[0]);
        FT_SetOPL3(reg + 0x40, (op[1] & 0xC0) | ((vol ^ 0x3F) & 0x3F));
        FT_SetOPL3(reg + 0x60, op[2]);
        FT_SetOPL3(reg + 0x80, op[3]);
        FT_SetOPL3(reg + 0xE0, op[4]);
    }
}

void FT_LoadInstMultiplierOPL3(uint8_t channum, uint8_t op, uint8_t mult)
{
    uint16_t reg = 0x20 + OpOffsets3[channum][op];
    FT_SetOPL3(reg, (FT_GetOPL3(reg) & 0xF0) | (mult & 15));
}

void FT_LoadInstVolumeOPL3(uint8_t channum, uint8_t op, uint8_t vol)
{
    uint16_t reg = 0x40 + OpOffsets3[channum][op];
    FT_SetOPL3(reg, (FT_GetOPL3(reg) & 0xC0) | ((vol & 0x3F) ^ 0x3F));
}

void FT_LoadInstFeedbackOPL3(uint8_t channum, uint8_t which, uint8_t fb)
{
    uint16_t reg;
    if (which == 0) {

        reg = 0xC0 + Chn2Offsets3[channum];
        FT_SetOPL3(reg, (FT_GetOPL3(reg) & 0x31) | ((fb & 7) << 1));

    } else if (which == 1) {

        reg = 0xC0 + ChanOffsets3[channum];
        FT_SetOPL3(reg, (FT_GetOPL3(reg) & 0x31) | ((fb & 7) << 1));
    }
}
