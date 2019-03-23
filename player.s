;
; RoboPlay
;
; Copyright (C) 2019 by RoboSoft.
;
; Made with FUSION-C in SDCC
;
    .globl  _FT_load
    .globl  _FT_update
    .globl  _FT_rewind
    .globl  _FT_getRefresh
    .globl  _FT_getSubSongs
    .globl  _FT_getPlayerInfo
    .globl  _FT_getTitle
    .globl  _FT_getAuthor
    .globl  _FT_getDescription

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

    .dw _FT_load
    .dw _FT_update
    .dw _FT_rewind
    .dw _FT_getRefresh
    .dw _FT_getSubSongs
    .dw _FT_getPlayerInfo
    .dw _FT_getTitle
    .dw _FT_getAuthor
    .dw _FT_getDescription

    .org    0x4030  ;RoboPlay player code start adress

    .area   _CODE
