;
; RoboPlay
;
; Copyright (C) 2019 by RoboSoft.
;
; Made with FUSION-C in SDCC
;
    .globl  _RP_Load
    .globl  _RP_Update
    .globl  _RP_Rewind
    .globl  _RP_GetRefresh
    .globl  _RP_GetSubSongs
    .globl  _RP_GetPlayerInfo
    .globl  _RP_GetTitle
    .globl  _RP_GetAuthor
    .globl  _RP_GetDescription

    .area   _HEADER (ABS)

    .org    0x4000

    .db 0x52    ;R
    .db 0x4F    ;O
    .db 0x42    ;B
    .db 0x4F    ;O
    .db 0x50    ;P
    .db 0x4C    ;L
    .db 0x41    ;A
    .db 0x59    ;Y

    .db 0x01    ; Major version
    .db 0x00    ; Minor version

    .dw _RP_Load
    .dw _RP_Update
    .dw _RP_Rewind
    .dw _RP_GetRefresh
    .dw _RP_GetSubSongs
    .dw _RP_GetPlayerInfo
    .dw _RP_GetTitle
    .dw _RP_GetAuthor
    .dw _RP_GetDescription

    .org    0x4050  ;RoboPlay player code start adress

    .area   _CODE
