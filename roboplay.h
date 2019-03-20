//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC

#ifndef __ROBOPLAY_H
#define __ROBOPLAY_H

#define PPI_REG_A   0xA8
#define PPI_REG_B   0xA9
#define PPI_REG_C   0xAA

#define ENASLT      0x24

#define OPL4_REG    0xC4
#define OPL4_DATA   0xC5
#define OPL4_REG2   0xC6
#define OPL4_DATA2  0xC7

#define OPL4_TIMER1_COUNT 0x02
#define OPL4_TIMER2_COUNT 0x03

#define MAX_NR_SEGMENTS 256

#define PAGE_0           0
#define PAGE_1           1   
#define PAGE_2           2
#define PAGE_3           3

typedef struct 
{
    byte allocatedSegmentNumber;
    byte slotAddressOfMapper;   
} SEGMENT_INFO;

typedef void (*fnPtr)(void);

#endif