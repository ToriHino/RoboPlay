//
// RoboPlay
//
// Copyright (C) 2020 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// OPL4 routines based on "ms2.c" by Avelino Herrera Morales
// http://msx.atlantes.org/moonsound/ms2.c

#include <stdio.h>
#include <stdint.h>

#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/newTypes.h"

#include "opl4.h"

#define OPL4_WAIT while(opl4_fm_base & 0x01);

boolean FT_OPL4_Detect()
{
    return (opl4_fm_base == 0xFF) ? false : true;
}

void FT_OPL4_FM1_Write(uint8_t reg, uint8_t data)
{
    OPL4_WAIT;
    opl4_fm_reg1 = reg;
    OPL4_WAIT;
    opl4_fm_data1 = data;
}

void FT_OPL4_FM2_Write(uint8_t reg, uint8_t data)
{
    OPL4_WAIT;
    opl4_fm_reg2 = reg;
    OPL4_WAIT;
    opl4_fm_data2 = data;
}

void FT_OPL4_WAVE_Write(uint8_t reg, uint8_t data)
{
    OPL4_WAIT;
    opl4_wave_reg = reg;
    OPL4_WAIT;
    opl4_wave_data = data;
}

void FT_OPL4_WAVE_StartDataWrite()
{
    OPL4_WAIT;
    opl4_wave_reg = 6;
}

void FT_OPL4_WAVE_Data(uint8_t data)
{
    opl4_wave_data = data;
}

uint8_t FT_OPL4_Read()
{
    OPL4_WAIT;
    return (uint8_t)opl4_fm_base;
}

uint8_t FT_OPL4_WAVE_Read(uint8_t reg)
{
    OPL4_WAIT;
    opl4_wave_reg = reg;
    OPL4_WAIT;
    return (uint8_t)opl4_wave_data;
}

uint8_t FT_OPL4_DetectRam()
{
    uint8_t i, j;

     // Set custom sample headers to 16Mb area and READ/WRITE mode
    FT_OPL4_WAVE_Write(0x02, 0x11);

    // Detect the number of 64K banks/blocks of RAM are installed
    for(i = 0; i < 32; i++)
    {
        // Write block number to corresponding OPL4 RAM address
        FT_OPL4_WAVE_Write(0x03, i + 0x20);  // register 3: memory address bits 16-21
        FT_OPL4_WAVE_Write(0x04, 0);         // register 4: memory address bits 8-5
        FT_OPL4_WAVE_Write(0x05, 0);         // register 5: memory address bits 0-7
        FT_OPL4_WAVE_Write(0x06, i + 123);  // 123 is just an abritrary magic number

        // Read back the number from the same location and check
        FT_OPL4_WAVE_Write(0x03, i + 0x20);  // register 3: memory address bits 16-21
        FT_OPL4_WAVE_Write(0x04, 0);         // register 4: memory address bits 8-5
        FT_OPL4_WAVE_Write(0x05, 0);         // register 5: memory address bits 0-7

        if(FT_OPL4_WAVE_Read(6) != (i + 123))
            return i;

        // See if one of the earlier written blocks has been overwritten
        for(j = i; j <= i; j--)
        {
            // Read back the number from the same location and check
            FT_OPL4_WAVE_Write(0x03, j + 0x20);  // register 3: memory address bits 16-21
            FT_OPL4_WAVE_Write(0x04, 0);         // register 4: memory address bits 8-5
            FT_OPL4_WAVE_Write(0x05, 0);         // register 5: memory address bits 0-7

            if(FT_OPL4_WAVE_Read(6) != (j + 123))
                return i;
        }
    }

    // Set custom sample headers to 16Mb area and PLAY mode
    FT_OPL4_WAVE_Write(0x02, 0x10);

    return i;
}

void FT_OPL4_Init()
{
    uint8_t i, val;

    // Set to OPL4 mode
    FT_OPL4_FM2_Write(5, 3);
    
    FT_OPL4_FM1_Write(1, 0);
    FT_OPL4_FM1_Write(2, 0);
    FT_OPL4_FM1_Write(3, 0);
    FT_OPL4_FM1_Write(4, 0);
    FT_OPL4_FM1_Write(8, 0);

    FT_OPL4_FM2_Write(1, 0);
    FT_OPL4_FM2_Write(2, 0);
    FT_OPL4_FM2_Write(3, 0);
    FT_OPL4_FM2_Write(4, 0);
    FT_OPL4_FM2_Write(8, 0);

    for(i = 20; i < 0xf6; i++)
    {
        val = (i >= 0x60 && i < 0xA0) ? 0xFF : 0;
        FT_OPL4_FM1_Write(i, val);
        FT_OPL4_FM2_Write(i, val);
    }

    for(i = 0; i < 23; i++)
        FT_OPL4_WAVE_Write(0x68 + i, 0);

    FT_OPL4_FM1_Write(4, 0x80);
}
