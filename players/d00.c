//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// D00: EdLib packed module player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "d00.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

#define HIBYTE(val) (val >> 8)
#define LOBYTE(val) (val & 0xff)

static const unsigned char op_table[9] =
    {0x00, 0x01, 0x02, 0x08, 0x09, 0x0a, 0x10, 0x11, 0x12};

static const uint16_t notetable[12] = // D00 note table
    {340,363,385,408,432,458,485,514,544,577,611,647};

static inline uint16_t LE_WORD(const uint16_t *val)
{
  const uint8_t *b = (const uint8_t *)val;
  return (b[1] << 8) + b[0];
}

boolean RP_Load(char* fileName)
{  
    int8_t i;
    uint8_t* str;
    uint16_t bytesRead;
    boolean ver1 = false;

    iRoboPlay = (void *)ROBO_PLAYER_BASE;

    iRoboPlay->RP_Open(fileName);

    filedata = (uint8_t*)D00_FILE_DATA;
    bytesRead = iRoboPlay->RP_Read(filedata, D00_MAX_FILE_SIZE);
    iRoboPlay->RP_Close();

    filedata[bytesRead] = 0x00;
    header = (D00_HEADER*)filedata;

    // Check for version 2-4 header
    if(strncmp(header->id, "JCH\x26\x02\x66",6) || header->type || !header->subsongs || header->soundcard)
    {
        // Check for version 0 or 1 header
        header1 = (D00_HEADER_1*)filedata;
        if(header1->version > 1 || !header1->subsongs) return false;
        ver1 = true;
    }

    if(!ver1)   // Version 2 and above
    {
        version = header->version;
        datainfo = (uint8_t *)(filedata + LE_WORD(&header->infoptr));
        inst = (INSTS *)(filedata + LE_WORD(&header->instptr));
        seqptr = (uint16_t *)(filedata + LE_WORD(&header->seqptr));

        for(i = 31; i >= 0; i--)
        {
            if(header->songname[i] == ' ') header->songname[i] = '\0';
            if(header->author[i] == ' ') header->author[i] = '\0';
        }
    }
    else    // Version 1
    {
        version = header1->version;
        datainfo = (uint8_t *)(filedata + LE_WORD(&header1->infoptr));
        inst = (INSTS *)(filedata + LE_WORD(&header1->instptr));
        seqptr = (uint16_t *)(filedata + LE_WORD(&header1->seqptr));          
    }

    switch(version)
    {
        case 0:
            levpuls = 0;
            spfx = 0;
            header1->speed = 70;    // V0 files deafult to 70Hz
            break;
        case 1:
            levpuls = (LEVPULS *)(filedata + LE_WORD(&header1->lpulptr));
            spfx = 0;
            break;
        case 2:
            levpuls = (LEVPULS *)(filedata + LE_WORD(&header->spfxptr));
            spfx = 0;
            break;
        case 3:
            spfx = 0;
            levpuls = 0;
            break;
        case 4:
            spfx = (SPFX *)(filedata + LE_WORD(&header->spfxptr));
            levpuls = 0;
            break;
    }

    if((str = strstr(datainfo, "\xff\xff")))
    {
        while((*str == '\xff' || *str == ' ') && str >= datainfo)
        {
            *str = '\0'; str--;
        }
    }

    return true;
}

boolean RP_Update()
{
    uint8_t  c, cnt, trackend=0, fx, note;
    uint16_t ord, *patt, buf, fxop, pattpos;

    // effect handling (timer dependant)
    for(c=0; c<9; c++) 
    {
        channel[c].slideval += channel[c].slide; 
        FT_SetFreq(c); // sliding
        FT_Vibrato(c); // vibrato

        if(channel[c].spfx != 0xffff) 
        { // SpFX
            if(channel[c].fxdel)
                channel[c].fxdel--;
            else 
            {
                channel[c].spfx = LE_WORD(&spfx[channel[c].spfx].ptr);
                channel[c].fxdel = spfx[channel[c].spfx].duration;
                channel[c].inst = LE_WORD(&spfx[channel[c].spfx].instnr) & 0xfff;
                if(spfx[channel[c].spfx].modlev != 0xff)
                    channel[c].modvol = spfx[channel[c].spfx].modlev;
                FT_SetInst(c);
                if(LE_WORD(&spfx[channel[c].spfx].instnr) & 0x8000) // locked frequency
                    note = spfx[channel[c].spfx].halfnote;
                else                            // unlocked frequency
                    note = spfx[channel[c].spfx].halfnote + channel[c].note;
                channel[c].freq = notetable[note%12] + ((note/12) << 10);
                FT_SetFreq(c);
          }
          channel[c].modvol += spfx[channel[c].spfx].modlevadd; channel[c].modvol &= 63;
          FT_SetVolume(c);
        }

        if(channel[c].levpuls != 0xff) 
        {    // Levelpuls
            if(channel[c].frameskip)
                channel[c].frameskip--;
            else 
            {
                channel[c].frameskip = inst[channel[c].inst].timer;
                if(channel[c].fxdel)
                    channel[c].fxdel--;
                else 
                {
                    channel[c].levpuls = levpuls[channel[c].levpuls].ptr - 1;
                    channel[c].fxdel = levpuls[channel[c].levpuls].duration;
                    if(levpuls[channel[c].levpuls].level != 0xff)
                        channel[c].modvol = levpuls[channel[c].levpuls].level;
                }
                channel[c].modvol += levpuls[channel[c].levpuls].voladd; channel[c].modvol &= 63;
                FT_SetVolume(c);
            }
        }
    }

    // song handling
    for(c=0; c<9; c++)
    {
        if(version < 3 ? channel[c].del : channel[c].del <= 0x7f) 
        {
            if(version == 4)  // v4: hard restart SR
                if(channel[c].del == inst[channel[c].inst].timer)
                    if(channel[c].nextnote)
                        iRoboPlay->RP_WriteOpl1(0x83 + op_table[c], inst[channel[c].inst].sr);
            if(version < 3)
                channel[c].del--;
            else if(channel[c].speed)
                channel[c].del += channel[c].speed;
            else 
            {
                channel[c].seqend = 1;
                continue;
            }
        }
        else 
        {
            if(channel[c].speed) 
            {
                if(version < 3)
                    channel[c].del = channel[c].speed;
                else 
                {
                    channel[c].del &= 0x7f;
                    channel[c].del += channel[c].speed;
                }
            }
            else 
            {
                channel[c].seqend = 1;
                continue;
            }
            if(channel[c].rhcnt) 
            {    // process pending REST/HOLD events
                channel[c].rhcnt--;
                continue;
            }

            readorder:  // process arrangement (orderlist)
            ord = LE_WORD(&channel[c].order[channel[c].ordpos]);
            switch(ord) 
            {
                case 0xfffe: channel[c].seqend = 1; continue; // end of arrangement stream
                case 0xffff:      // jump to order
                    channel[c].ordpos = LE_WORD(&channel[c].order[channel[c].ordpos + 1]);
                    channel[c].seqend = 1;
                    goto readorder;
                default:
                if(ord >= 0x9000) 
                { // set speed
                    channel[c].speed = ord & 0xff;
                    ord = LE_WORD(&channel[c].order[channel[c].ordpos - 1]);
                    channel[c].ordpos++;
                } 
                else if(ord >= 0x8000) 
                {   // transpose track
                    channel[c].transpose = ord & 0xff;
                    if(ord & 0x100)
                        channel[c].transpose = -channel[c].transpose;
                    ord = LE_WORD(&channel[c].order[++channel[c].ordpos]);
            }

            patt = (uint16_t *)(filedata + LE_WORD(&seqptr[ord]));
            break;
        }

        channel[c].fxflag = 0;
        
        readseq:    // process sequence (pattern)
        if(!version)  // v0: always initialize rhcnt
            channel[c].rhcnt = channel[c].irhcnt;
        pattpos = LE_WORD(&patt[channel[c].pattpos]);
        if(pattpos == 0xffff) 
        {   // pattern ended?
            channel[c].pattpos = 0;
            channel[c].ordpos++;
            goto readorder;
        }

        cnt = HIBYTE(pattpos);
        note = LOBYTE(pattpos);
        fx = pattpos >> 12;
        fxop = pattpos & 0x0fff;
        channel[c].pattpos++; pattpos = LE_WORD(&patt[channel[c].pattpos]);
        channel[c].nextnote = LOBYTE(pattpos) & 0x7f;
      
        if(version ? cnt < 0x40 : !fx) 
        {  // note event
            switch(note) 
            {
                case 0:                     // REST event
                case 0x80:
                    if(!note || version) 
                    {
                        channel[c].key = 0;
                        FT_SetFreq(c);
                    }
                    // fall through...
                case 0x7e:                  // HOLD event
                    if(version)
                        channel[c].rhcnt = cnt;
                    channel[c].nextnote = 0;
                    break;
                default:                    // play note
                    // restart fx
                    if(!(channel[c].fxflag & 1))
                        channel[c].vibdepth = 0;
                    if(!(channel[c].fxflag & 2))
                        channel[c].slideval = channel[c].slide = 0;

                    if(version)
                    { // note handling for v1 and above
                        if(note > 0x80) // locked note (no channel transpose)
                            note -= 0x80;
                        else            // unlocked note
                            note += channel[c].transpose;
                        channel[c].note = note; // remember note for SpFX

                        if(channel[c].ispfx != 0xffff && cnt < 0x20) 
                        {  // reset SpFX
                            channel[c].spfx = channel[c].ispfx;
                            if(LE_WORD(&spfx[channel[c].spfx].instnr) & 0x8000)   // locked frequency
                                note = spfx[channel[c].spfx].halfnote;
                            else                                              // unlocked frequency
                                note += spfx[channel[c].spfx].halfnote;
                            channel[c].inst = LE_WORD(&spfx[channel[c].spfx].instnr) & 0xfff;
                            channel[c].fxdel = spfx[channel[c].spfx].duration;
                            if(spfx[channel[c].spfx].modlev != 0xff)
                                channel[c].modvol = spfx[channel[c].spfx].modlev;
                            else
                                channel[c].modvol = inst[channel[c].inst].data[7] & 63;
                        }

                        if(channel[c].ilevpuls != 0xff && cnt < 0x20) 
                        { // reset LevelPuls
                            channel[c].levpuls = channel[c].ilevpuls;
                            channel[c].fxdel = levpuls[channel[c].levpuls].duration;
                            channel[c].frameskip = inst[channel[c].inst].timer;
                            if(levpuls[channel[c].levpuls].level != 0xff)
                                channel[c].modvol = levpuls[channel[c].levpuls].level;
                            else
                                channel[c].modvol = inst[channel[c].inst].data[7] & 63;
                        }

                        channel[c].freq = notetable[note%12] + ((note/12) << 10);
                        if(cnt < 0x20)  // normal note
                            FT_PlayNote(c);
                        else 
                        {          // tienote
                            FT_SetFreq(c);
                            cnt -= 0x20;  // make count proper
                        }
                        channel[c].rhcnt = cnt;
                    } 
                    else 
                    {  // note handling for v0
                        if(cnt < 2) // unlocked note
                            note += channel[c].transpose;
                        channel[c].note = note;

                        channel[c].freq = notetable[note%12] + ((note/12) << 10);
                        if(cnt == 1)    // tienote
                            FT_SetFreq(c);
                        else            // normal note
                            FT_PlayNote(c);
                    }
                    break;
                }
                continue;   // event is complete
            } 
            else
            {      // effect event
                switch(fx) 
                {
                    case 6:     // Cut/Stop Voice
                        buf = channel[c].inst;
                        channel[c].inst = 0;
                        FT_PlayNote(c);
                        channel[c].inst = buf;
                        channel[c].rhcnt = fxop;
                        continue; // no note follows this event
                    case 7:     // Vibrato
                        channel[c].vibspeed = fxop & 0xff;
                        channel[c].vibdepth = fxop >> 8;
                        channel[c].trigger = fxop >> 9;
                        channel[c].fxflag |= 1;
                        break;
                    case 8:     // v0: Duration
                        if(!version)
                            channel[c].irhcnt = fxop;
                        break;
                    case 9:     // New Level
                        channel[c].vol = fxop & 63;
                        if(channel[c].vol + channel[c].cvol < 63) // apply channel volume
                            channel[c].vol += channel[c].cvol;
                        else
                            channel[c].vol = 63;
                        FT_SetVolume(c);
                        break;
                    case 0xb:   // v4: Set SpFX
                        if(version == 4)
                            channel[c].ispfx = fxop;
                        break;
                    case 0xc:   // Set Instrument
                        channel[c].ispfx = 0xffff;
                        channel[c].spfx = 0xffff;
                        channel[c].inst = fxop;
                        channel[c].modvol = inst[fxop].data[7] & 63;
                        if(version < 3 && version && inst[fxop].tunelev)  // Set LevelPuls
                            channel[c].ilevpuls = inst[fxop].tunelev - 1;
                        else 
                        {
                            channel[c].ilevpuls = 0xff;
                            channel[c].levpuls = 0xff;
                        }
                        break;
                    case 0xd:   // Slide up
                        channel[c].slide = fxop;
                        channel[c].fxflag |= 2;
                        break;
                    case 0xe:   // Slide down
                        channel[c].slide = -fxop;
                        channel[c].fxflag |= 2;
                        break;
                }
                goto readseq;   // event is incomplete, note follows
            }
        }
    }

    for(c=0; c<9; c++)
    {
        if(channel[c].seqend)
            trackend++;
        if(trackend == 9)
            songend = 1;
    }

    return !songend;
}

void RP_Rewind(int8_t subsong)
{
    uint8_t i;
    TPOIN* tpoin;

    // Set to standard OPL2 mode
    iRoboPlay->RP_WriteOpl2(0x05, 0);

    if(subsong == -1) subsong = cursubsong;

    if(version > 1)     // do nothing if subsong > number of subsongs
    { 
        if(subsong >= header->subsongs)
            return;
    } else
        if(subsong >= header1->subsongs)
            return;

    memset(channel, 0, sizeof(channel));
    if(version > 1)
        tpoin = (TPOIN*)(filedata + LE_WORD(&header->tpoin));
    else
        tpoin = (TPOIN*)(filedata + LE_WORD(&header1->tpoin));

    for(i=0; i<9; i++) 
    {
        if(LE_WORD(&tpoin[subsong].ptr[i]))     // track enabled
        {
            channel[i].speed = LE_WORD((uint16_t*)
                 (filedata + LE_WORD(&tpoin[subsong].ptr[i])));
            channel[i].order = (uint16_t*)
                (filedata + LE_WORD(&tpoin[subsong].ptr[i]) + 2);
        } else 
        {                    // track disabled
            channel[i].speed = 0;
            channel[i].order = 0;
        }
        channel[i].ispfx = 0xffff; channel[i].spfx = 0xffff;    // no SpFX
        channel[i].ilevpuls = 0xff; channel[i].levpuls = 0xff;  // no LevelPuls
        channel[i].cvol = tpoin[subsong].volume[i] & 0x7f;      // our player may safely ignore bit 7
        channel[i].vol = channel[i].cvol;                       // initialize volume
    }
    songend = 0;
    cursubsong = subsong;
}

float RP_GetRefresh()
{
    if(version > 1)
        return header->speed;
    else
        return header1->speed;
}

uint8_t RP_GetSubSongs()
{
    if(version > 1)
        return header->subsongs;
    else
        return header1->subsongs;
}

char* RP_GetPlayerInfo()
{
    return "EdLib packed module player by RoboSoft Inc.";
}

char* RP_GetTitle()
{
    if(version > 1)
        return header->songname;
    else
        return "-";
}

char* RP_GetAuthor()
{
    if(version > 1)
        return header->author;
    else
        return "-";
}

char* RP_GetDescription()
{
    if(*datainfo)
        return datainfo;
    else
        return "-";
}

void FT_SetVolume(uint8_t chan)
{
  uint8_t  op = op_table[chan];
  uint16_t insnr = channel[chan].inst;

  iRoboPlay->RP_WriteOpl1(
        0x43 + op,
        (uint8_t)(63-((63-(inst[insnr].data[2] & 63))/63.0)*(63-channel[chan].vol)) + (inst[insnr].data[2] & 192));

  if(inst[insnr].data[10] & 1)
    iRoboPlay->RP_WriteOpl1(
        0x40 + op,
        (uint8_t)(63-((63-channel[chan].modvol)/63.0)*(63-channel[chan].vol)) + (inst[insnr].data[7] & 192));
  else
    iRoboPlay->RP_WriteOpl1(
        0x40 + op,
        channel[chan].modvol + (inst[insnr].data[7] & 192));
}

void FT_SetFreq(uint8_t chan)
{
  uint16_t freq = channel[chan].freq;

  if(version == 4)  // v4: apply instrument finetune
    freq += inst[channel[chan].inst].tunelev;

  freq += channel[chan].slideval;
  iRoboPlay->RP_WriteOpl1(0xa0 + chan, freq & 255);
  if(channel[chan].key)
    iRoboPlay->RP_WriteOpl1(0xb0 + chan, ((freq >> 8) & 31) | 32);
  else
    iRoboPlay->RP_WriteOpl1(0xb0 + chan, (freq >> 8) & 31);
}

void FT_SetInst(uint8_t chan)
{
  uint8_t  op = op_table[chan];
  uint16_t insnr = channel[chan].inst;

  // set instrument data
  iRoboPlay->RP_WriteOpl1(0x63 + op, inst[insnr].data[0]);
  iRoboPlay->RP_WriteOpl1(0x83 + op, inst[insnr].data[1]);
  iRoboPlay->RP_WriteOpl1(0x23 + op, inst[insnr].data[3]);
  iRoboPlay->RP_WriteOpl1(0xe3 + op, inst[insnr].data[4]);
  iRoboPlay->RP_WriteOpl1(0x60 + op, inst[insnr].data[5]);
  iRoboPlay->RP_WriteOpl1(0x80 + op, inst[insnr].data[6]);
  iRoboPlay->RP_WriteOpl1(0x20 + op, inst[insnr].data[8]);
  iRoboPlay->RP_WriteOpl1(0xe0 + op, inst[insnr].data[9]);
  if(version)
    iRoboPlay->RP_WriteOpl1(0xc0 + chan, inst[insnr].data[10]);
  else
    iRoboPlay->RP_WriteOpl1(0xc0 + chan, (inst[insnr].data[10] << 1) + (inst[insnr].tunelev & 1));
}

void FT_PlayNote(uint8_t chan)
{
  // set misc vars & play
  iRoboPlay->RP_WriteOpl1(0xb0 + chan, 0);   // stop old note
  FT_SetInst(chan);
  channel[chan].key = 1;
  FT_SetFreq(chan);
  FT_SetVolume(chan);
}

void FT_Vibrato(uint8_t chan)
{
  if(!channel[chan].vibdepth)
    return;

  if(channel[chan].trigger)
    channel[chan].trigger--;
  else {
    channel[chan].trigger = channel[chan].vibdepth;
    channel[chan].vibspeed = -channel[chan].vibspeed;
  }
  channel[chan].freq += channel[chan].vibspeed;
  FT_SetFreq(chan);
}
