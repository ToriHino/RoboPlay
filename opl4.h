//
// RoboPlay
//
// Copyright (C) 2020 by RoboSoft.
//
// Made with FUSION-C in SDCC
//

#ifndef __OPL_4_H
#define __OPL_4_H

__sfr __at 0xC4 opl4_fm_base;
__sfr __at 0x7E opl4_wave_base;
__sfr __at 0xC4 opl4_status;

__sfr __at 0xC4 opl4_fm_reg1;
__sfr __at 0xC5 opl4_fm_data1;
__sfr __at 0xC6 opl4_fm_reg2;
__sfr __at 0xC7 opl4_fm_data2;

__sfr __at 0x7E opl4_wave_reg;
__sfr __at 0x7F opl4_wave_data;

boolean FT_OPL4_Detect();

void FT_OPL4_FM1_Write(uint8_t reg, uint8_t data);
void FT_OPL4_FM2_Write(uint8_t reg, uint8_t data);
void FT_OPL4_WAVE_Write(uint8_t reg, uint8_t data);

void FT_OPL4_WAVE_StartDataWrite();
void FT_OPL4_WAVE_Data(uint8_t data);

uint8_t FT_OPL4_Read();
uint8_t FT_OPL4_WAVE_Read(uint8_t reg);

uint8_t FT_OPL4_DetectRam();
void FT_OPL4_Init();

#endif