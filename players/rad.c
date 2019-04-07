//
// RoboPlay
//
// Copyright (C) 2019 by RoboSoft.
//
// Made with FUSION-C in SDCC
//
// DRO2: DOSBox Raw OPL v2.0 player

#include <string.h>
#include <stdint.h>
#include "../fusion-c/header/msx_fusion.h"
#include "../fusion-c/header/newTypes.h"

#include "../player.h"
#include "rad.h"

ROBO_PLAYER_INTERFACE* iRoboPlay;

#include "protrack.c"

boolean RP_Load(char* fileName)
{  
    iRoboPlay = (void *)ROBO_PLAYER_BASE;

    iRoboPlay->RP_Open(fileName);
    iRoboPlay->RP_Close();

    return true;
}

float RP_GetRefresh()
{
    // Fixed replay rate of 1000Hz
    return 1000.0;
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