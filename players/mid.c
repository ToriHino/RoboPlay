//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// MIDI & MIDI-like file player

#include <stdint.h>
#include "../fusion-c/Header/msx_fusion.h"
#include "../fusion-c/Header/newTypes.h"

#include "../player.h"
#include "mid.h"

const uint8_t adlib_opadd[] = {0x00  ,0x01 ,0x02  ,0x08  ,0x09  ,0x0A  ,0x10 ,0x11  ,0x12};
const uint8_t map_chan[] = { 0x14, 0x12, 0x15, 0x11 };
const uint16_t fnums[] = { 0x16b,0x181,0x198,0x1b0,0x1ca,0x1e5,0x202,0x220,0x241,0x263,0x287,0x2ae };
const uint8_t percussion_map[] = { 6, 7, 8, 8, 7 };


ROBO_PLAYER_INTERFACE* iRoboPlay;

boolean RP_Load(char* fileName)
{
    uint8_t good = 0;
    unsigned char s[6];

    iRoboPlay = (void *)ROBO_PLAYER_BASE;

    iRoboPlay->RP_Open(fileName);
    iRoboPlay->RP_Read(s, 6);

    subsongs = 0;
    switch(s[0])
    {
        case 'M':
            if (s[1]=='T' && s[2]=='h' && s[3]=='d') good = FILE_MIDI;
            break;
    }

    if(good != 0) subsongs = 1;
    else
    {
        iRoboPlay->RP_Close();
        return false;
    }

    FileData = (uint8_t*)SEGMENT_BASE;
    iRoboPlay->RP_Read(FileData, SEGMENT_SIZE);
    iRoboPlay->RP_Close();

    return true;  
}

boolean RP_Update()
{
    boolean ret = true;
    uint8_t w, v, note, vel, ctrl, nv, x, l, lnum, numchan;
    uint8_t i = 0, j, c;
    int8_t on, onl;

    if(doing)
    {
        for(curtrack = 0; curtrack < 16; curtrack++)
        {
            if(track[curtrack].on)
            {
                pos = track[curtrack].pos;
                track[curtrack].iwait += FT_GetNext(1);
                track[curtrack].pos = pos;
            }
        }
        doing = false;
    }

    iwait = 0;
    while(iwait == 0 && ret == true)
    {
        for (curtrack = 0; curtrack < 16; curtrack++)
        if (track[curtrack].on && track[curtrack].iwait == 0 &&
            track[curtrack].pos < track[curtrack].tend)
        {
            pos = track[curtrack].pos;

            v = FT_GetNext(1);

            //  This is to do implied MIDI events.
            if (v < 0x80) { v = track[curtrack].pv; pos--; }
            track[curtrack].pv=(unsigned char)v;

            c = v & 0x0f;
            switch(v & 0xf0)
            {
                case 0x80: /*note off*/
                    note = FT_GetNext(1); 
                    vel = FT_GetNext(1);
                    for (i=0; i<9; i++)
                        if (chp[i][0] == c && chp[i][1] == note)
                        {
                            FT_MIDI_FM_EndNote(i);
                            chp[i][0]=-1;
                        }
                    break;

                case 0x90: /*note on*/
                    note = FT_GetNext(1);
                    vel = FT_GetNext(1);

                    if(adlib_mode == ADLIB_RYTHM)
                        numchan = 6;
                    else
                        numchan = 9;

                    if(ch[c].on != 0)
                    {
                        for(i = 0; i < 18; i++)
                            chp[i][2]++;

                        if(c < 11 || adlib_mode == ADLIB_MELODIC) 
                        {
                            j = 0;
                            on = -1; onl=0;
                            for (i = 0; i < numchan; i++)
                                if(chp[i][0] == -1 && chp[i][2]>onl)
                                { 
                                    onl = chp[i][2];
                                    on = i; j = 1; 
                                }

                            if (on==-1)
                            {
                                onl = 0;
                                for(i = 0; i < numchan; i++)
                                    if(chp[i][2] > onl)
                                    { 
                                        onl = chp[i][2];
                                        on = i; }
                                    }

                            if (j==0)
                                FT_MIDI_FM_EndNote(on);
                        } else
                            on = percussion_map[c - 11];

                        if (vel != 0 && ch[c].inum >= 0 && ch[c].inum <128) 
                        {
                            if (adlib_mode == ADLIB_MELODIC || c < 12) // 11 == bass drum, handled like a normal instrument, on == channel 6 thanks to percussion_map[] above
                                FT_MIDI_FM_Instrument(on,ch[c].ins);
//                            else
//                                FT_MIDI_FM_PERCUSSION(c, ch[c].ins);

                            nv = ((ch[c].vol * vel) / 128);
                            if(nv > 127) nv = 127;
                            nv = my_midi_fm_vol_table[nv];

                            FT_MIDI_FM_PlayNote(on, note+ch[c].nshift, nv*2); // sets freq in rhythm mode
                            chp[on][0] = c;
                            chp[on][1] = note;
                            chp[on][2] = 0;

                            if(adlib_mode == ADLIB_RYTHM && c >= 11) 
                            {
                                // Still need to turn off the perc instrument before playing it again,
                                // as not all songs send a noteoff.
                                FT_MIDI_Write_Adlib(0xbd, adlib_data[0xbd] & ~(0x10 >> (c - 11)));
                                // Play the perc instrument
                                FT_MIDI_Write_Adlib(0xbd, adlib_data[0xbd] | (0x10 >> (c - 11)));
                            }
                        } else 
                        {
                            if (vel==0) 
                            { //same code as end note
                                if (adlib_mode == ADLIB_RYTHM && c >= 11) 
                                {
                                    // Turn off the percussion instrument
                                    FT_MIDI_Write_Adlib(0xbd, adlib_data[0xbd] & ~(0x10 >> (c - 11)));
                                    chp[percussion_map[c - 11]][0]=-1;
                                } else 
                                {
                                    for(i = 0; i < 9; i++)
                                    {
                                        if(chp[i][0] == c && chp[i][1] == note)
                                        {
                                            FT_MIDI_FM_EndNote(i);
                                            chp[i][0]=-1;
                                        }
                                    }
                                }
                            } else 
                            {
                                // i forget what this is for.
                                chp[on][0]=-1;
                                chp[on][2]=0;
                            }
                        }
                    }
                    break;

                case 0xa0: /*key after touch */
                    note = FT_GetNext(1);
                    vel = FT_GetNext(1);
                    break;

                case 0xb0: /*control change .. pitch bend? */
                    ctrl = FT_GetNext(1); 
                    vel = FT_GetNext(1);

                    switch(ctrl)
                    {
                        case 0x07:
                            ch[c].vol = vel;
                            break;
                    }
                    break;

                case 0xc0: /*patch change*/
                    x = FT_GetNext(1);
                    ch[c].inum = x;
                    for (j = 0; j < 11; j++)
                        ch[c].ins[j] = midi_fm_instruments[ch[c].inum][j];
                    break;

                case 0xd0: /*chanel touch*/
                    x = FT_GetNext(1);
                    break;

                case 0xe0: /*pitch wheel*/
                    x = FT_GetNext(1);
                    x = FT_GetNext(1);
                    break;

                case 0xf0:
                    switch(v)
                    {
                        case 0xf0:
                        case 0xf7: /*sysex*/
                            l = FT_GetVal();
                            if (FT_DataLook(pos+l) == 0xf7)
                                i=1;

                            if (FT_DataLook(pos)==0x7d &&
                                FT_DataLook(pos+1)==0x10 &&
                                FT_DataLook(pos+2)<16)
                            {
                                FT_GetNext(1);
                                FT_GetNext(1);
                                c = FT_GetNext(1);
                                FT_GetNext(1);

                                ch[c].ins[0]=(unsigned char)((FT_GetNext(1)<<4)+FT_GetNext(1));
                                ch[c].ins[2]=(unsigned char)(0xff-(((FT_GetNext(1)<<4)+FT_GetNext(1))&0x3f));
                                ch[c].ins[4]=(unsigned char)(0xff-((FT_GetNext(1)<<4)+FT_GetNext(1)));
                                ch[c].ins[6]=(unsigned char)(0xff-((FT_GetNext(1)<<4)+FT_GetNext(1)));
                                ch[c].ins[8]=(unsigned char)((FT_GetNext(1)<<4)+FT_GetNext(1));

                                ch[c].ins[1]=(unsigned char)((FT_GetNext(1)<<4)+FT_GetNext(1));
                                ch[c].ins[3]=(unsigned char)(0xff-(((FT_GetNext(1)<<4)+FT_GetNext(1))&0x3f));
                                ch[c].ins[5]=(unsigned char)(0xff-((FT_GetNext(1)<<4)+FT_GetNext(1)));
                                ch[c].ins[7]=(unsigned char)(0xff-((FT_GetNext(1)<<4)+FT_GetNext(1)));
                                ch[c].ins[9]=(unsigned char)((FT_GetNext(1)<<4)+FT_GetNext(1));

                                i = (FT_GetNext(1) << 4) + FT_GetNext(1);
                                ch[c].ins[10] = i;

                                FT_GetNext(l - 26);
                            }
                            else
                            {
                                for (j=0; j<l; j++)
                                    FT_GetNext(1);
                            }

                            if(i==1)
                                FT_GetNext(1);
                            break;
                        case 0xf1:
                            break;
                        case 0xf2:
                            FT_GetNext(2);
                            break;
                        case 0xf3:
                            FT_GetNext(1);
                            break;
                        case 0xf4:
                            break;
                        case 0xf5:
                            break;
                        case 0xf6: /*something*/
                        case 0xf8:
                        case 0xfa:
                        case 0xfb:
                        case 0xfc:
                            break;
                        case 0xfe:
                            break;
                        case 0xfd:
                            break;
                        case 0xff:
                            v = FT_GetNext(1);
                            l = FT_GetVal();
                            if(v == 0x51)
                            {
                                lnum = FT_GetNext(l);
                                msqtr = lnum; /*set tempo*/
                            }
                            else
                            {
                                for (i=0; i<l; i++)
                                    FT_GetNext(1);
                            }
                            break;
                    }
                    break;

                default: /* if we get down here, a error occurred */
                    break;
            }

            if (pos < track[curtrack].tend)
            {
                w = FT_GetVal();
                track[curtrack].iwait = w;
            }
            else
                track[curtrack].iwait = 0;

            track[curtrack].pos = pos;
        }


        ret = false; //end of song.
        iwait = 0;
        for(curtrack = 0; curtrack < 16; curtrack++)
            if (track[curtrack].on == 1 &&
                track[curtrack].pos < track[curtrack].tend)
                ret = true;  //not yet..

        if(ret == true)
        {
            iwait=0xffffff;  // bigger than any wait can be!
            for(curtrack = 0; curtrack < 16; curtrack++)
                if(track[curtrack].on == 1 &&
                   track[curtrack].pos < track[curtrack].tend &&
                   track[curtrack].iwait < iwait)
                    iwait = track[curtrack].iwait;
        }
    }

    if(iwait !=0 && ret==true)
    {
        for(curtrack = 0; curtrack < 16; curtrack++)
            if(track[curtrack].on)
                track[curtrack].iwait -= iwait;
        
        fwait =1.0f/(((float)iwait/(float)deltas)*((float)msqtr/(float)1000000));
    }
    else
        fwait = 50;  // 1/50th of a second

    return ret;
}

void RP_Rewind(int8_t subsong)
{
    uint8_t i;

    // No subsongs in this format
    subsong;

    pos = 0;
    adlib_mode = ADLIB_MELODIC;
    for(i = 0; i < 16; i++)
    {
        ch[i].inum = 0;
        ch[i].vol = 127;
        ch[i].nshift = -25;
        ch[i].on = 1;
    }

    for(i = 0; i < 9; i++)
    {
        chp[i][0] = -1;
        chp[i][2] = 0;
    }

    deltas = 250;   // Just a number, not a standard
    msqtr = 500000;
    fwait = 123;    // Gotta be a small thing.. sorta like nothing
    iwait = 0;

    for (i = 0; i < 16; i++)
    {
        track[i].tend = 0;
        track[i].spos = 0;
        track[i].pos = 0;
        track[i].iwait = 0;
        track[i].on = 0;
        track[i].pv = 0;
    }
    curtrack = 0;

    FT_GetNext(12);
    deltas = FT_GetNext(2);
    FT_GetNext(4);
    curtrack = 0;
    track[curtrack].on = 1;
    track[curtrack].tend = FT_GetNext(4);
    track[curtrack].spos = pos;

    for (i = 0; i < 16; i++)
    {
        if(track[i].on)
        {
            track[i].pos = track[i].spos;
            track[i].pv = 0;
            track[i].iwait = 0;
        }
    }

    doing = true;
    FT_MIDI_Reset();
}

float RP_GetRefresh()
{
//    return (fwait > 0.01 ? fwait : 0.01);
    return 50.0;
}

uint8_t RP_GetSubSongs()
{
    return 0;
}

char* RP_GetPlayerInfo()
{
    return "MIDI & MIDI-like file player by RoboSoft Inc.";
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

uint8_t FT_DataLook(uint16_t pos)
{
    return FileData[pos];
}

uint32_t FT_GetNext(uint8_t num)
{
    uint32_t v = 0;
    uint8_t i = 0;

    for(i = 0; i < num; i++)
    {
        v <<= 8;
        v += FT_DataLook(pos); pos++;
    }
    return v;
}

uint32_t FT_GetVal()
{
    uint32_t v = 0;
    uint8_t b = 0;

    b = FT_GetNext(1);
    v = b & 0x7f;
    while ((b & 0x80) !=0)
    {
        b= FT_GetNext(1);
        v = (v << 7) + (b & 0x7F);
    }

    return v;
}

void FT_MIDI_Write_Adlib(uint8_t reg, uint8_t val)
{
    iRoboPlay->RP_WriteOpl1(reg, val);
    adlib_data[reg] = val;
}

void FT_MIDI_FM_Instrument(uint8_t voice, uint8_t* inst)
{
    FT_MIDI_Write_Adlib(0x20 + adlib_opadd[voice], inst[0]);
    FT_MIDI_Write_Adlib(0x23 + adlib_opadd[voice], inst[1]);

    FT_MIDI_Write_Adlib(0x40 + adlib_opadd[voice], inst[2]);
    if((inst[10] & 1) == 0)
        FT_MIDI_Write_Adlib(0x43 + adlib_opadd[voice], inst[3]);
    else
        FT_MIDI_Write_Adlib(0x43 + adlib_opadd[voice], 0);

    FT_MIDI_Write_Adlib(0x60 + adlib_opadd[voice],inst[4]);
    FT_MIDI_Write_Adlib(0x63 + adlib_opadd[voice],inst[5]);
    FT_MIDI_Write_Adlib(0x80 + adlib_opadd[voice],inst[6]);
    FT_MIDI_Write_Adlib(0x83 + adlib_opadd[voice],inst[7]);
    FT_MIDI_Write_Adlib(0xe0 + adlib_opadd[voice],inst[8]);
    FT_MIDI_Write_Adlib(0xe3 + adlib_opadd[voice],inst[9]);

    FT_MIDI_Write_Adlib(0xc0+voice,inst[10]);
}

void FT_MIDI_FM_Percussion(uint8_t ch, uint8_t* inst)
{
    uint8_t opadd = map_chan[ch - 12];

    FT_MIDI_Write_Adlib(0x20 + opadd, inst[0]);
    FT_MIDI_Write_Adlib(0x40 + opadd, inst[2]);
    FT_MIDI_Write_Adlib(0x60 + opadd, inst[4]);
    FT_MIDI_Write_Adlib(0x80 + opadd, inst[6]);
    FT_MIDI_Write_Adlib(0xe0 + opadd, inst[8]);

    // Only output this for the modulator, not the carrier, as it affects the entire channel
    if (opadd < 0x13) 
        FT_MIDI_Write_Adlib(0xc0 + percussion_map[ch - 11], inst[10]);
}

void FT_MIDI_FM_Volume(uint8_t voice, uint8_t volume)
{
    uint8_t vol;

    vol = volume >> 2;   
    if ((adlib_data[0xc0 + voice] & 1) ==1)
            FT_MIDI_Write_Adlib(0x40 + adlib_opadd[voice], (uint8_t)((63-vol) |
                                (adlib_data[0x40 + adlib_opadd[voice]] & 0xc0)));

    FT_MIDI_Write_Adlib(0x43 + adlib_opadd[voice], (unsigned char)((63-vol) |
                        (adlib_data[0x43 + adlib_opadd[voice]] & 0xc0)));
}

void FT_MIDI_FM_PlayNote(uint8_t voice, uint8_t note, uint8_t volume)
{
    uint8_t freq = fnums[note % 12];
    uint8_t oct= note/12;
    uint8_t c;

    FT_MIDI_FM_Volume(voice, volume);
    FT_MIDI_Write_Adlib(0xa0 + voice,(uint8_t)(freq & 0xff));

    c = ((freq & 0x300) >> 8) + ((oct & 7) <<2) + (adlib_mode == ADLIB_MELODIC || voice < 6 ? (1 << 5) : 0);
    FT_MIDI_Write_Adlib(0xb0 + voice, c);
}

void FT_MIDI_FM_EndNote(uint8_t voice)
{
    FT_MIDI_Write_Adlib(0xb0 + voice,(uint8_t)(adlib_data[0xb0 + voice] & (255-32)));
}

void FT_MIDI_Reset()
{
    int i;

    for (i = 0; i < 256; i++) FT_MIDI_Write_Adlib(i, 0);

    FT_MIDI_Write_Adlib(0x01, 0x20);
    FT_MIDI_Write_Adlib(0xBD, 0xC0);
}