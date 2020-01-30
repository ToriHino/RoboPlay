;--- Patch table ---

patch_table:	
	.dw	patch_0
	.dw	patch_1
	.dw	patch_2A, patch_2B
	.dw	patch_0
	.dw	patch_3B
	.dw	patch_4
	.dw	patch_5A, patch_5B
	.dw	patch_6
	.dw	patch_7
	.dw	patch_8
	.dw	patch_9
	.dw	patch_10A, patch_10B
	.dw	patch_11
	.dw	patch_12
	.dw	patch_13
	.dw	patch_14
	.dw	patch_15A, patch_15B
	.dw	patch_16
	.dw	patch_17
	.dw	patch_18
	.dw	patch_19
	.dw	patch_20
	.dw	patch_21A, patch_21B
	.dw	patch_22
	.dw	patch_23A, patch_23B
	.dw	patch_24
	.dw	patch_25
	.dw	patch_26
	.dw	patch_27
	.dw	patch_28
	.dw	patch_29
	.dw	patch_30
	.dw	patch_31
	.dw	patch_32
	.dw	patch_33
	.dw	patch_34
	.dw	patch_35
	.dw	patch_36
	.dw	patch_37
	.dw	patch_38
	.dw	patch_39
	.dw	patch_40
	.dw	patch_41
	.dw	patch_42
	.dw	patch_43
	.dw	patch_44
	.dw	patch_45
	.dw	patch_46
	.dw	patch_47
	.dw	patch_48
	.dw	patch_49
	.dw	patch_50A, patch_50B
	.dw	patch_51A, patch_51B
	.dw	patch_52
	.dw	patch_53
	.dw	patch_54
	.dw	patch_55
	.dw	patch_56
	.dw	patch_57
	.dw	patch_58
	.dw	patch_59
	.dw	patch_60
	.dw	patch_61
	.dw	patch_62A, patch_62B
	.dw	patch_63A, patch_63B
	.dw	patch_64
	.dw	patch_65
	.dw	patch_66
	.dw	patch_67
	.dw	patch_68
	.dw	patch_69
	.dw	patch_70
	.dw	patch_71
	.dw	patch_72
	.dw	patch_73
	.dw	patch_74
	.dw	patch_75
	.dw	patch_76A,patch_76B
	.dw	patch_77A,patch_77B
	.dw	patch_78
	.dw	patch_80A, patch_80B
	.dw	patch_81A, patch_81B
	.dw	patch_82A, patch_82B
	.dw	patch_83A, patch_83B
	.dw	patch_84A, patch_84B
	.dw	patch_85A, patch_85B
	.dw	patch_86A, patch_86B
	.dw	patch_87A, patch_84B
	.dw	patch_88A, patch_88B
	.dw	patch_89A, patch_89B
	.dw	patch_90A, patch_90B
	.dw	patch_91A, patch_91B
	.dw	patch_92A, patch_92B
	.dw	patch_93A, patch_93B
	.dw	patch_94A, patch_94B
	.dw	patch_95A, patch_95B
	.dw	patch_79
	.dw	patch_96
	.dw	patch_97A, patch_97B
	.dw	patch_98A, patch_98B
	.dw	patch_99A, patch_99B
	.dw	patch_100A, patch_100B
	.dw	patch_102A, patch_102B
	.dw	patch_103A, patch_103B
	.dw	patch_104
	.dw	patch_105
	.dw	patch_106
	.dw	patch_107
	.dw	patch_109A,patch_109B
	.dw	patch_108
	.dw	patch_110
	.dw	patch_111
	.dw	patch_112A,patch_112B
	.dw	patch_113
	.dw	patch_114A,patch_114B
	.dw	patch_115
	.dw	patch_116
	.dw	patch_117
	.dw	patch_118
	.dw	patch_119
	.dw	patch_120
	.dw	patch_121
	.dw	patch_122A,patch_122B
	.dw	patch_124
	.dw	patch_125A,patch_125B
	.dw	patch_126A,patch_126B
	.dw	patch_127
	.dw	drum_1
	.dw	drum_2
	.dw	drum_3
	.dw	drum_4
	.dw	drum_5
	.dw	drum_6
	.dw	drum_7
	.dw	drum_8
	.dw	drum_midi


;--- GM 0 = Ac. Piano ---

patch_0:	
 	.db	1		; transpose on
	.dw	patch_0hdr
	.db	16,0x2c,1 + 36 * 2	; tone 300
	.dw	frq_842
	.db	22,0x2d,1 + 52 * 2	; tone 301
	.dw	frq_363
	.db	28,0x2e,1 + 46 * 2	; tone 302
	.dw	frq_810
	.db	34,0x2f,1 + 52 * 2	; tone 303
	.dw	frq_369
	.db	40,0x30,1 + 46 * 2	; tone 304
	.dw	frq_726
	.db	46,0x31,1 + 52 * 2	; tone 305
	.dw	frq_290
	.db	52,0x32,1 + 58 * 2	; tone 306
	.dw	frq_21
	.db	59,0x33,1 + 52 * 2	; tone 307
	.dw	frq_286
	.db	65,0x34,1 + 47 * 2	; tone 308
	.dw	frq_941
	.db	255,0x35,1 + 65 * 2	; tone 309
	.dw	frq_191

patch_0hdr:	.db 0x20,0xff


;--- GM 1 = Bright Acoustic Piano ---

patch_1:	.db	1		; transpose on
	.dw	patch_0hdr
	.db	22,0x2c,1 + 36 * 2	; tone 300
	.dw	frq_842
	.db	28,0x2d,1 + 58 * 2	; tone 301
	.dw	frq_363
	.db	34,0x2e,1 + 52 * 2	; tone 302
	.dw	frq_809
	.db	40,0x2f,1 + 58 * 2	; tone 303
	.dw	frq_369
	.db	46,0x30,1 + 52 * 2	; tone 304
	.dw	frq_726
	.db	52,0x31,1 + 58 * 2	; tone 305
	.dw	frq_290
	.db	59,0x32,1 + 64 * 2	; tone 306
	.dw	frq_21
	.db	65,0x33,1 + 59 * 2	; tone 307
	.dw	frq_286
	.db	71,0x34,1 + 53 * 2	; tone 308
	.dw	frq_941
	.db	255,0x35,1 + 71 * 2	 ; tone 309
	.dw	frq_191


;--- GM 2 = Elec. Grand Piano ---
; A = v104
; B = v98

patch_2A:	.db	1		; transpose on
	.dw	patch_2Ahdr
	.db	22,0x2c,1 + 36 * 2	; tone 300
	.dw	frq_844
	.db	28,0x2d,1 + 58 * 2	; tone 301
	.dw	frq_364
	.db	34,0x2e,1 + 52 * 2	; tone 302
	.dw	frq_811
	.db	40,0x2f,1 + 58 * 2	; tone 303
	.dw	frq_370
	.db	46,0x30,1 + 52 * 2	; tone 304
	.dw	frq_727
	.db	52,0x31,1 + 58 * 2	; tone 305
	.dw	frq_291
	.db	59,0x32,1 + 64 * 2	; tone 306
	.dw	frq_22
	.db	65,0x33,1 + 59 * 2	; tone 307
	.dw	frq_287
	.db	71,0x34,1 + 53 * 2	; tone 308
	.dw	frq_942
	.db	255,0x35,1 + 71 * 2	 ; tone 309
	.dw	frq_192

patch_2Ahdr:	.db 0x20,0xc8,0x07,0xff

patch_2B:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	255,108,36 * 2
	.dw	frq_113

patch_2Bhdr:	.db 0x28,0xff


;--- GM 3 = Honky Tonk, met patch#0 ---

patch_3B:	.db	1		; transpose on
	.dw	patch_0hdr
	.db	16,0x2c,1 + 36 * 2	; tone 300
	.dw	frq_852
	.db	22,0x2d,1 + 52 * 2	; tone 301
	.dw	frq_355
	.db	28,0x2e,1 + 46 * 2	; tone 302
	.dw	frq_820
	.db	34,0x2f,1 + 52 * 2	; tone 303
	.dw	frq_360
	.db	40,0x30,1 + 46 * 2	; tone 304
	.dw	frq_737
	.db	46,0x31,1 + 52 * 2	; tone 305
	.dw	frq_282
	.db	52,0x32,1 + 58 * 2	; tone 306
	.dw	frq_27
	.db	59,0x33,1 + 52 * 2	; tone 307
	.dw	frq_282
	.db	65,0x34,1 + 47 * 2	; tone 308
	.dw	frq_952
	.db	255,0x35,1 + 65 * 2	 ; tone 309
	.dw	frq_184

;--- GM 4 = Elec. Piano 1 (+ 2B?) ---

patch_4:	.db	1		; transpose on
	.dw	patch_4hdr
	.db	255,108,36 * 2
	.dw	frq_113

patch_4hdr:	.db 0x22,0xc8,0x48,0xff


;--- GM 5 = Elec. Piano 2 ---

patch_5A:	.db	1		; transpose on
	.dw	patch_5Ahdr
	.db	16,0x2c,1 + 36 * 2	; tone 300
	.dw	frq_844
	.db	22,0x2d,1 + 52 * 2	; tone 301
	.dw	frq_364
	.db	28,0x2e,1 + 46 * 2	; tone 302
	.dw	frq_811
	.db	34,0x2f,1 + 52 * 2	; tone 303
	.dw	frq_370
	.db	40,0x30,1 + 46 * 2	; tone 304
	.dw	frq_727
	.db	46,0x31,1 + 52 * 2	; tone 305
	.dw	frq_291
	.db	52,0x32,1 + 58 * 2	; tone 306
	.dw	frq_22
	.db	59,0x33,1 + 52 * 2	; tone 307
	.dw	frq_287
	.db	65,0x34,1 + 47 * 2	; tone 308
	.dw	frq_942
	.db	255,0x35,1 + 65 * 2	 ; tone 309
	.dw	frq_192

patch_5Ahdr:	.db 0x1b,0xff

patch_5B:	.db	1		; transpose on
	.dw	patch_5Ahdr
	.db	22,0x2c,1 + 36 * 2	; tone 300
	.dw	frq_842
	.db	28,0x2d,1 + 58 * 2	; tone 301
	.dw	frq_363
	.db	34,0x2e,1 + 52 * 2	; tone 302
	.dw	frq_809
	.db	40,0x2f,1 + 58 * 2	; tone 303
	.dw	frq_369
	.db	46,0x30,1 + 52 * 2	; tone 304
	.dw	frq_726
	.db	52,0x31,1 + 58 * 2	; tone 305
	.dw	frq_290
	.db	59,0x32,1 + 64 * 2	; tone 306
	.dw	frq_21
	.db	65,0x33,1 + 59 * 2	; tone 307
	.dw	frq_286
	.db	71,0x34,1 + 53 * 2	; tone 308
	.dw	frq_941
	.db	255,0x35,1 + 71 * 2	 ; tone 309
	.dw	frq_191


;--- GM 6 = Harpsichord ---

patch_6:	.db	1		; transpose on
	.dw	patch_0hdr
	.db	34,128,24 * 2
	.dw	frq_288
	.db	40,129,46 * 2
	.dw	frq_847
	.db	46,130,52 * 2
	.dw	frq_288
	.db	54,131,46 * 2
	.dw	frq_823
	.db	255,132,42 * 2
	.dw	frq_823


;--- GM 7 = Clavi ---

patch_7:	.db	1		; transpose on
	.dw	patch_7hdr
	.db	58,39,24 * 2
	.dw	frq_203
	.db	255,40,70 * 2
	.dw	frq_215
patch_7hdr:	.db 0x28,0xc8,0x2b,0xff


;--- GM 8 = Celesta ---

patch_8:	.db	1		; transpose on
	.dw	patch_8hdr
	.db	255,43,12 * 2
	.dw	frq_94
patch_8hdr:	.db 0x20,0xe0,3,0xff


;--- GM 9 = Glockenspiel ---

patch_9:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	255,243,0x0 * 2
	.dw	frq_141

;--- GM 10 = Music Box ---

patch_10A:	.db	1		; transpose on
	.dw	patch_10Ahdr
	.db	255,243,12 * 2
	.dw	frq_143
patch_10Ahdr:	.db 0x20,0x98,0xa6,0xff

patch_10B:	.db	1		; transpose on
	.dw	patch_10Bhdr
	.db	255,1,1 + 24 * 2		 ; tone 257
	.dw	frq_79
patch_10Bhdr:	.db 0x20,0x98,0xd4,0xff


;--- GM 11 = Vibraphone ---

patch_11:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	255,1,1 + 24 * 2		 ; tone 257
	.dw	frq_82

;--- GM 12 = Marimba ---

patch_12:	.db	1		; transpose on
	.dw	patch_12hdr
	.db	40,244, 24 * 2
	.dw	frq_82
	.db	255,245, 52 * 2		; was andere header!
	.dw	frq_69
patch_12hdr:	.db 0x38,0xff

;--- GM 13 = Xylophone ---

patch_13:	.db	1		; transpose on
	.dw	patch_12hdr
	.db	255,0x36, 1 + 0 * 2	 ; tone 310
	.dw	frq_94


;--- GM 14 = Tubular Bells ---

patch_14:	.db	1		; transpose on
	.dw	patch_14hdr
	.db	255,255, 12 * 2
	.dw	frq_532
patch_14hdr:	.db 0x21,0xe0,1,0xff

;--- GM 15 = Dulcimer ---

patch_15A:	.db	1		; transpose on
	.dw	patch_15Ahdr
	.db	255,63, 12 * 2
	.dw	frq_708
patch_15Ahdr:	.db 0x29,0xff
patch_15B:	.db	1
	.dw	patch_15Bhdr
	.db	255,64, 12 * 2
	.dw	frq_708
patch_15Bhdr:	.db 0x2a,0xff

;--- GM 16 = Drawbar organ ---

patch_16:	.db	1		; transpose on
	.dw	patch_16hdr
	.db	255,142,12 * 2
	.dw	frq_835
patch_16hdr:	.db 0x3a,0xff

;--- GM 17 = Percussive organ ---

patch_17:	.db	1		; transpose on
	.dw	patch_17hdr
	.db	36,140, 24 * 2
	.dw	frq_950
	.db	255,141, 36 * 2
	.dw	frq_944
patch_17hdr:	.db 0x3b,0xff

;--- GM 18 = Rock Organ ---

patch_18:	.db	1		; transpose on
	.dw	patch_18hdr
	.db	25,0x28, 1 + 36 * 2	; tone 296
	.dw	frq_219
	.db	37,0x29, 1 + 49 * 2	; tone 297
	.dw	frq_220
	.db	49,0x2a, 1 + 49 * 2	; tone 298
	.dw	frq_217
	.db	61,0x2b, 1 + 49 * 2	; tone 299
	.dw	frq_214
	.db	255,0x27, 1 + 49 * 2	 ; tone 295
	.dw	frq_215
patch_18hdr:	.db 0x39,0xff

;--- GM 19 = Church organ ---

patch_19:	.db	1		; transpose on
	.dw	patch_19hdr
	.db	18,135, 36 * 2
	.dw	frq_835
	.db	25,136, 54 * 2
	.dw	frq_155
	.db	33,137, 49 * 2
	.dw	frq_459
	.db	42,138, 45 * 2
	.dw	frq_847
	.db	255,139, 54 * 2
	.dw	frq_167
patch_19hdr:	.db 0x11,0xff


;--- GM 20 = Reed Organ ---

patch_20:	.db	1		; transpose on
	.dw	patch_20hdr
	.db	60,172, 24 * 2
	.dw	frq_557
	.db	255,173, 60 * 2
	.dw	frq_556
patch_20hdr:	.db 0x38,0xe0,1,0xff

;--- GM 21 = Accordion ---

patch_21A:	.db	1		; transpose on
	.dw	patch_21Ahdr
	.db	53,6, 12 * 2
	.dw	frq_727
	.db	255,7, 41 * 2
	.dw	frq_1018
patch_21Ahdr:	.db 0x22,0xe0,0x,0xff

patch_21B:	.db	1		; transpose on
	.dw	patch_21Bhdr
	.db	255,112, 12 * 2
	.dw	frq_833
patch_21Bhdr:	.db 0x23,0xff

;--- GM 22 = Harmonica ---

patch_22:	.db	1		; transpose on
	.dw	patch_22hdr
	.db	255,112, 12 * 2
	.dw	frq_847
patch_22hdr:	.db 0x30,0xe0,2,0xff

;--- GM 23 = Tango Accordion ---

patch_23A:	.db	1		; transpose on
	.dw	patch_23Ahdr
	.db	60,172, 24 * 2
	.dw	frq_559
	.db	255,173, 60 * 2
	.dw	frq_558
patch_23Ahdr:	.db 0x38,0xff
patch_23B:	.db	1
	.dw	patch_23Bhdr
	.db	255,65, 48 * 2
	.dw	frq_445
patch_23Bhdr:	.db 0x3a,0x98,0x75,0xB0,0x00,0xff

;--- GM 24 = Nylon Ac. Gtr ---

patch_24:	.db	1		; transpose on
	.dw	patch_24hdr
	.db	24,179, 36 * 2
	.dw	frq_459
	.db	31,183, 48 * 2
	.dw	frq_555
	.db	37,181, 55 * 2
	.dw	frq_21
	.db	44,180, 49 * 2
	.dw	frq_459
	.db	255,182, 44 * 2
	.dw	frq_726
patch_24hdr:	.db 0x28,0xc8,9,0xff


;--- GM 26 = Steel Ac. Gtr ---

patch_25:	.db	1		; transpose on
	.dw	patch_25hdr
	.db	26,12,36 * 2	;was andere header
	.dw	frq_442
	.db	33,13,50 * 2	; ""
	.dw	frq_448
	.db	48,14,45 * 2	; ""
	.dw	frq_823
	.db	255,15,48 * 2	; ""
	.dw	frq_823
patch_25hdr:	.db 0x28,0x09,0xf6,0xb0,0x34,0xc8,0x09,0xff


;--- GM 26 = Elec Jazz Gtr ---

patch_26:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	26,90, 36 * 2
	.dw	frq_373
	.db	40,91, 50 * 2
	.dw	frq_143
	.db	255,92, 52 * 2
	.dw	frq_70


;--- GM 27 = Elec Clean Gtr ---

patch_27:	.db	1		; transpose on
	.dw	patch_27hdr
	.db	21,97, 36 * 2
	.dw	frq_519
	.db	26,96, 57 * 2
	.dw	frq_143
	.db	33,99, 50 * 2
	.dw	frq_701
	.db	40,98, 57 * 2
	.dw	frq_9
	.db	45,101, 52 * 2
	.dw	frq_508
	.db	52,100, 57 * 2
	.dw	frq_118
	.db	61,102, 52 * 2
	.dw	frq_531
	.db	255,103, 49 * 2
	.dw	frq_823
patch_27hdr:	.db 0x29,0xff


;--- GM 28 = Muted Gtr ---

patch_28:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	255,104, 12 * 2
	.dw	frq_847


;--- GM 29 = Overdriven Gtr ---

patch_29:	.db	1		; transpose on
	.dw	patch_27hdr
	.db	41,165, 36 * 2
	.dw	frq_228
	.db	255,166, 65 * 2
	.dw	frq_459

;--- GM 30 = Distortion Gtr ---

patch_30:	.db	1		; transpose on
	.dw	patch_30hdr
	.db	19,81, 36 * 2
	.dw	frq_434
	.db	23,82, 55 * 2
	.dw	frq_143
	.db	27,83, 47 * 2
	.dw	frq_847
	.db	31,84, 51 * 2
	.dw	frq_434
	.db	35,85, 55 * 2
	.dw	frq_143
	.db	39,86, 47 * 2
	.dw	frq_847
	.db	43,87, 51 * 2
	.dw	frq_459
	.db	47,88, 55 * 2
	.dw	frq_143
	.db	255,89, 47 * 2
	.dw	frq_775
patch_30hdr:	.db 0x21,0xff

;--- GM 31 = Gtr Harmonics ---

patch_31:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	45,94, 24 * 2
	.dw	frq_507
	.db	50,93, 69 * 2
	.dw	frq_118
	.db	255,95, 62 * 2
	.dw	frq_726

;--- GM 32 = Acoustic Bass ---

patch_32:	.db	1		; transpose on
	.dw	patch_32hdr
	.db	25,4, 48 * 2
	.dw	frq_188
	.db	255,5, 49 * 2
	.dw	frq_70
patch_32hdr:	.db 0x18,0xff


;--- GM 33 = Elec. Bass (finger) ---

patch_33:	.db	1		; transpose on
	.dw	patch_32hdr
	.db	 9,74,48 * 2
	.dw	frq_645
	.db	14,75,57 * 2
	.dw	frq_221
	.db	19,76,50 * 2
	.dw	frq_843
	.db	255,77,55 * 2
	.dw	frq_378


;--- GM 34 = Elec. Bass (pick) ---

patch_34:	.db	1		; transpose on
	.dw	patch_32hdr
	.db	12,79,48 * 2
	.dw	frq_135
	.db	19,80,48 * 2
	.dw	frq_715
	.db	24,107,55 * 2
	.dw	frq_264
	.db	48,105,48 * 2
	.dw	frq_921
	.db	255,106,72 * 2
	.dw	frq_434

;--- GM 35 = Fretless Bass ---

patch_35:	.db	1		; transpose on
	.dw	patch_35hdr
	.db	255,78,24 * 2
	.dw	frq_373
patch_35hdr:	.db 0x10,0x98,0xf3,0xb0,0x63,0xc8,0x19,0xff

;--- GM 36 = Slap Bass 1 ---

patch_36:	.db	1		; transpose on
	.dw	patch_36hdr
	.db	255,163,(36 + 3) * 2
	.dw	frq_641

patch_36hdr:	.db 0x19,0xff

;--- GM 37 = Slap bass 2 ---

patch_37:	.db	1		; transpose on
	.dw	patch_37hdr
	.db	255,162,24 * 2
	.dw	frq_288
patch_37hdr:	.db 0x20,0x98,0xb0,0xff

;--- GM 38 = Synth Bass 1 ---

patch_38:	.db	1		; transpose on
	.dw	patch_38hdr
	.db	255,190,36 * 2
	.dw	frq_835
patch_38hdr:	.db 0x39,0xff

;--- GM 39 = Synth Bass 2 ---

patch_39:	.db	1		; transpose on
	.dw	patch_38hdr
	.db	255,0x17,1 + 48 * 2	; tone 279
	.dw	frq_215

;--- GM 40 = Violin ---

patch_40:	.db	1		; transpose on
	.dw	patch_40hdr
	.db	35,0x5,1 + 24 * 2		; tone 261
	.dw	frq_288
	.db	40,0x2,1 + 59 * 2		; tone 258
	.dw	frq_69
	.db	42,0x6,1 + 52 * 2		; tone 262
	.dw	frq_627
	.db	45,0x7,1 + 54 * 2		; tone 263
	.dw	frq_556
	.db	48,0x8,1 + 57 * 2		; tone 264
	.dw	frq_264
	.db	51,0xa,1 + 48 * 2 	; tone 266
	.dw	frq_920
	.db	53,0xb,1 + 51 * 2 	; tone 267
	.dw	frq_726
	.db	55,0xc,1 + 53 * 2 	; tone 268
	.dw	frq_579
	.db	58,0xd,1 + 55 * 2 	; tone 269
	.dw	frq_338
	.db	255,0x9,1 + 58  * 2	 ; tone 265
	.dw	frq_143
patch_40hdr:	.db 0x3b,0xff


;--- GM 41 = Viola ---

patch_41:	.db	1		; transpose on
	.dw	patch_41hdr
	.db	27,0x3,1 + 24 * 2 	; tone 259
	.dw	frq_715
	.db	30,0x4,1 + 51 * 2 	; tone 260
	.dw	frq_531
	.db	33,0x5,1 + 54 * 2 	; tone 261
	.dw	frq_288
	.db	38,0x2,1 + 57 * 2 	; tone 258
	.dw	frq_69
	.db	40,0x6,1 + 50 * 2 	; tone 262
	.dw	frq_627
	.db	43,0x7,1 + 52 * 2 	; tone 263
	.dw	frq_556
	.db	46,0x8,1 + 55 * 2 	; tone 264
	.dw	frq_264
	.db	49,0xa,1 + 46 * 2 	; tone 266
	.dw	frq_920
	.db	51,0xb,1 + 49 * 2 	; tone 267
	.dw	frq_726
	.db	54,0xc,1 + 51 * 2 	; tone 268
	.dw	frq_579
	.db	56,0xd,1 + 54 * 2 	; tone 269
	.dw	frq_338
	.db	255,0x9,1 + 56 * 2	 ; tone 265
	.dw	frq_143
patch_41hdr:	.db 0x3b,0x98,0xa3,0xff


;--- GM 42 = Cello ---

patch_42:	.db	1		; transpose on
	.dw	patch_42hdr
	.db	22,0x12,1 + 36 * 2 	; tone 274
	.dw	frq_203
	.db	32,0x13,1 + 46 * 2 	; tone 275
	.dw	frq_701
	.db	39,0x15,1 + 44 * 2 	; tone 277
	.dw	frq_822
	.db	45,0x16,1 + 51 * 2 	; tone 278
	.dw	frq_531
	.db	255,0x14,1 + 57 * 2	 ; tone 276
	.dw	frq_69
patch_42hdr:	.db 0x33,0xc8,8,0xff

;--- GM 43 = Contrabass ---

patch_43:	.db	1		; transpose on
	.dw	patch_43hdr
	.db	18,0x10,1 + 48 * 2 	; tone 272
	.dw	frq_15
	.db	255,0x11,1 + 54 * 2	 ; tone 273
	.dw	frq_8
patch_43hdr:	.db 0x10,0x98,0x90,0xff

;--- GM 44 = Tremolo Strings ---

patch_44:	.db	1		; transpose on
	.dw	patch_44hdr
	.db	36,176,24 * 2
	.dw	frq_98
	.db	42,53,48 * 2
	.dw	frq_557
	.db	48,51,54 * 2
	.dw	frq_26
	.db	59,52,48 * 2
	.dw	frq_650
	.db	255,175,47 * 2
	.dw	frq_538
patch_44hdr:	.db 0x38,0xe0,6,0xff


;--- GM 45 = Pizzicato Strings ---

patch_45:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	27,184,36 * 2
	.dw	frq_20
	.db	36,185,51 * 2
	.dw	frq_216
	.db	43,187,48 * 2
	.dw	frq_639
	.db	49,186,55 * 2
	.dw	frq_84
	.db	255,188,49 * 2
	.dw	frq_455

;--- GM 46 = Harp ---

patch_46:	.db	1		; transpose on
	.dw	patch_46hdr
	.db	47,126,12 * 2
	.dw	frq_360
	.db	255,127,47 * 2
	.dw	frq_434
patch_46hdr:	.db 0x29,0xc8,7,0xff


;--- GM 47 = Timpani ---

patch_47:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	255,0x0,1 + 36 * 2		 ; tone 256
	.dw	frq_217


;--- GM 48 = Strings ---

patch_48:	.db	1		; transpose on
	.dw	patch_0hdr
	.db	36,0x3c,1 + 24 * 2	; tone 316
	.dw	frq_96
	.db	42,0x3e,1 + 48 * 2        ; tone 318
	.dw	frq_557
	.db	48,0x3d,1 + 54 * 2        ; tone 317
	.dw	frq_24
	.db	59,0x3f,1 + 48 * 2        ; tone 319
	.dw	frq_653
	.db	255,0x40,1 + 47 * 2	; tone 320
	.dw	frq_548

;--- GM 49 = Slow Strings ---

patch_49:	.db	1		; transpose on
	.dw	patch_49hdr
	.db	36,176, 24 * 2
	.dw	frq_98
	.db	42,53, 48 * 2
	.dw	frq_557
	.db	48,51, 54 * 2
	.dw	frq_27
	.db	59,52, 48 * 2
	.dw	frq_650
	.db	255,175, 47 * 2
	.dw	frq_538
patch_49hdr:	.db 0x19,0xff


;--- GM 50 = Synth Strings 1 ---

patch_50A:	.db	1		; transpose on
	.dw	patch_50Ahdr
	.db	255,2, 24 * 2
	.dw	frq_935
patch_50Ahdr:	.db 0x20,0x98,0x93,0xb0,0x22,0xc8,0x6,0xff
patch_50B:	.db	1		; transpose on
	.dw	patch_50Bhdr
	.db	255,174, 12 * 2
	.dw	frq_91
patch_50Bhdr:	.db 0x20,0x98,0x70,0xc8,0x06,0xff


;--- GM 51 = Synth Strings 2 ---

patch_51A:	.db	1		; transpose on
	.dw	patch_51Ahdr
	.db	255,2, 12 * 2
	.dw	frq_935
patch_51Ahdr:	.db 0x19,0x98,0x70,0xb0,0x00,0xc8,0x6,0xff
patch_51B:	.db	1		; transpose on
	.dw	patch_51Ahdr
	.db	255,2, 12 * 2
	.dw	frq_927


;--- GM 52 = Choir Aahs ---

patch_52:	.db	1		; transpose on
	.dw	patch_52hdr
	.db	35,24, 24 * 2
	.dw	frq_204
	.db	41,25, 47 * 2
	.dw	frq_815
	.db	48,26, 53 * 2
	.dw	frq_206
	.db	255,27, 48 * 2
	.dw	frq_356
patch_52hdr:	.db 0x1a,0xff

;--- GM 53 = Voice Oohs ---

patch_53:	.db	1		; transpose on
	.dw	patch_53hdr
	.db	255,41, 12 * 2
	.dw	frq_273
patch_53hdr:	.db 0x20,0x98,0xf7,0xb0,0x20,0xc8,8,0xff

;--- GM 54 = Synth Voice ---

patch_54:	.db	1		; transpose on
	.dw	patch_54hdr
	.db	255,42, 12 * 2
	.dw	frq_208
patch_54hdr:	.db 0x19,0x98,0x85,0xb0,0x21,0xc8,7,0xff

;--- GM 55 = Orchestra Hit ---

patch_55:	.db	1		; transpose on
	.dw	patch_55hdr
	.db	255,73, 12 * 2
	.dw	frq_835
patch_55hdr:	.db 0x30,0xff

;--- GM 56 = Trumpet ---

patch_56:	.db	1		; transpose on
	.dw	patch_56hdr
	.db	37,246, 24 * 2
	.dw	frq_46
	.db	44,248, 49 * 2
	.dw	frq_460
	.db	49,247, 56 * 2
	.dw	frq_22
	.db	55,250, 49 * 2
	.dw	frq_436
	.db	62,249, 55 * 2
	.dw	frq_46
	.db	255,251, 50 * 2
	.dw	frq_436
patch_56hdr:	.db 0x32,0xff

;--- GM 57 = Trombone ---

patch_57:	.db	1		; transpose on
	.dw	patch_57hdr
	.db	35,240, 24 * 2
	.dw	frq_228
	.db	40,241, 47 * 2
	.dw	frq_750
	.db	255,242, 52 * 2
	.dw	frq_264
patch_57hdr:	.db 0x21,0xff

;--- GM 58 = Tuba ---

patch_58:	.db	1		; transpose on
	.dw	patch_57hdr
	.db	22,133, 36 * 2
	.dw	frq_550
	.db	255,134, 46 * 2
	.dw	frq_908

;--- GM 59 = Muted Trumpet ---

patch_59:	.db	1		; transpose on
	.dw	patch_59hdr
	.db	46,177, 12 * 2
	.dw	frq_630
	.db	255,178, 46 * 2
	.dw	frq_630
patch_59hdr:	.db	0x28,0xe0,1,0xff

;--- GM 60 = French Horn ---

patch_60:	.db	1		; transpose on
	.dw	patch_60hdr
	.db	50,124, 12 * 2
	.dw	frq_290
	.db	255,125, 50 * 2
	.dw	frq_679
patch_60hdr:	.db 0x1a,0xff

;--- GM 61 = Brass Section ---

patch_61:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	43,252, 12 * 2
	.dw	frq_820
	.db	255,253, 43 * 2
	.dw	frq_832

;--- GM 62 = Synth Brass 1 ---

patch_62A:	.db	1		; transpose on
	.dw	patch_62Ahdr
	.db	16,211, 48 * 2
	.dw	frq_916
	.db	22,218, 64 * 2
	.dw	frq_354
	.db	28,212, 58 * 2
	.dw	frq_916
	.db	34,219, 64 * 2
	.dw	frq_345
	.db	40,213, 58 * 2
	.dw	frq_935
	.db	46,220, 64 * 2
	.dw	frq_350
	.db	52,214, 58 * 2
	.dw	frq_925
	.db	58,221, 64 * 2
	.dw	frq_363
	.db	64,215, 58 * 2
	.dw	frq_969
	.db	70,222, 64 * 2
	.dw	frq_338
	.db	77,216, 58 * 2
	.dw	frq_925
	.db	255,217, 53 * 2
	.dw	frq_925
patch_62Ahdr:	.db 0x18,0xff


patch_62B:	.db	1		; transpose on
	.dw	patch_62Bhdr
	.db	16,0x18, 1 + 48 * 2	; tone 280
	.dw	frq_929
	.db	22,0x19, 1 + 64 * 2	; tone 281
	.dw	frq_360
	.db	28,0x1a, 1 + 58 * 2	; tone 282
	.dw	frq_927
	.db	34,0x1b, 1 + 64 * 2	; tone 283
	.dw	frq_360
	.db	40,0x1c, 1 + 58 * 2	; tone 284
	.dw	frq_939
	.db	46,0x1d, 1 + 64 * 2	; tone 285
	.dw	frq_366
	.db	52,0x1e, 1 + 58 * 2	; tone 286
	.dw	frq_951
	.db	58,0x1f, 1 + 64 * 2	; tone 287
	.dw	frq_366
	.db	64,0x20, 1 + 58 * 2	; tone 288
	.dw	frq_977
	.db	70,0x21, 1 + 64 * 2	; tone 289
	.dw	frq_342
	.db	77,0x22, 1 + 58 * 2	; tone 290
	.dw	frq_927
	.db	255,0x23, 1 + 53 * 2	 ; tone 291
	.dw	frq_927
patch_62Bhdr:	.db 0x18,0x98,0xa5,0xb0,0x11,0xff


;--- GM 63 = Synth Brass 2 ---
; B zachter dan A, vals??

patch_63A:	.db	1		; transpose on
	.dw	patch_63Ahdr
	.db	16,0x18, 1 + 48 * 2	; tone 280
	.dw	frq_931
	.db	22,0x19, 1 + 64 * 2	; tone 281
	.dw	frq_361
	.db	28,0x1a, 1 + 58 * 2	; tone 282
	.dw	frq_929
	.db	34,0x1b, 1 + 64 * 2	; tone 283
	.dw	frq_361
	.db	40,0x1c, 1 + 58 * 2	; tone 284
	.dw	frq_941
	.db	46,0x1d, 1 + 64 * 2	; tone 285
	.dw	frq_367
	.db	52,0x1e, 1 + 58 * 2	; tone 286
	.dw	frq_953
	.db	58,0x1f, 1 + 64 * 2	; tone 287
	.dw	frq_367
	.db	64,0x20, 1 + 58 * 2	; tone 288
	.dw	frq_978
	.db	70,0x21, 1 + 64 * 2	; tone 289
	.dw	frq_343
	.db	77,0x22, 1 + 58 * 2	; tone 290
	.dw	frq_929
	.db	255,0x23, 1 + 53 * 2	 ; tone 291
	.dw	frq_929
patch_63Ahdr:	.db 0x26,0xff


patch_63B:	.db	1		; transpose on
	.dw	patch_63Bhdr
	.db	255,0x24, 1 + 12 * 2	 ; tone 292
	.dw	frq_557
patch_63Bhdr:	.db 0x22,0xff

;--- GM 64 = Soprano Sax ---

patch_64:	.db	1		; transpose on
	.dw	patch_30hdr
	.db	40,227, 12 * 2
	.dw	frq_701
	.db	46,228, 52 * 2
	.dw	frq_216
	.db	52,229, 46 * 2
	.dw	frq_677
	.db	58,230, 52 * 2
	.dw	frq_242
	.db	65,231, 46 * 2
	.dw	frq_726
	.db	255,232, 65 * 2
	.dw	frq_94

;--- GM 65 = Alto Sax ---

patch_65:	.db	1		; transpose on
	.dw	patch_65hdr
	.db	27,146, 36 * 2
	.dw	frq_27
	.db	30,150, 51 * 2
	.dw	frq_737
	.db	35,153, 54 * 2
	.dw	frq_385
	.db	36,143, 59 * 2
	.dw	frq_242
	.db	39,147, 60 * 2
	.dw	frq_46
	.db	42,151, 51 * 2
	.dw	frq_775
	.db	45,154, 54 * 2
	.dw	frq_460
	.db	48,144, 57 * 2
	.dw	frq_216
	.db	51,148, 60 * 2
	.dw	frq_22
	.db	53,152, 51 * 2
	.dw	frq_727
	.db	57,155, 53 * 2
	.dw	frq_480
	.db	60,145, 57 * 2
	.dw	frq_290
	.db	255,149, 60 * 2
	.dw	frq_94
patch_65hdr:	.db 0x19,0xc8,0x0b,0xff

;--- GM 66 = Tenor Sax ---

patch_66:	.db	1		; transpose on
	.dw	patch_36hdr
	.db	25,233, 36 * 2
	.dw	frq_75
	.db	29,234, 49 * 2
	.dw	frq_715
	.db	35,235, 53 * 2
	.dw	frq_228
	.db	42,237, 47 * 2
	.dw	frq_653
	.db	48,236, 54 * 2
	.dw	frq_70
	.db	58,238, 48 * 2
	.dw	frq_531
	.db	255,239, 46 * 2
	.dw	frq_920


;--- GM 67 = Baritone Sax ---

patch_67:	.db	1		; transpose on
	.dw	patch_36hdr
	.db	22,223, 36 * 2
	.dw	frq_299
	.db	29,225, 46 * 2
	.dw	frq_545
	.db	34,226, 53 * 2
	.dw	frq_299
	.db	255,224, 46 * 2
	.dw	frq_874

;--- GM 68 = Oboe ---

patch_68:	.db	1		; transpose on
	.dw	patch_68hdr
	.db	37,66, 12 * 2
	.dw	frq_920
	.db	44,68, 49 * 2
	.dw	frq_338
	.db	50,67, 44 * 2
	.dw	frq_920
	.db	56,70, 50 * 2
	.dw	frq_335
	.db	62,69, 44 * 2
	.dw	frq_920
	.db	255,71, 50 * 2
	.dw	frq_335
patch_68hdr:	.db 0x39,0xff

;--- GM 69 = English Horn ---

patch_69:	.db	1		; transpose on
	.dw	patch_69hdr
	.db	33,60, 24 * 2
	.dw	frq_254
	.db	39,59, 45 * 2
	.dw	frq_750
	.db	255,61, 51 * 2
	.dw	frq_242
patch_69hdr:	.db 0x31,0xff

;--- GM 70 = Bassoon ---

patch_70:	.db	1		; transpose on
	.dw	patch_69hdr
	.db	11,56, 48 * 2
	.dw	frq_73
	.db	23,58, 47 * 2
	.dw	frq_532
	.db	255,57, 59 * 2
	.dw	frq_75

;--- GM 71 = Clarinet ---

patch_71:	.db	1		; transpose on
	.dw	patch_71hdr
	.db	36,158, 24 * 2
	.dw	frq_810
	.db	42,160, 60 * 2
	.dw	frq_288
	.db	51,159, 54 * 2
	.dw	frq_726
	.db	255,161, 51 * 2
	.dw	frq_920
patch_71hdr:	.db 0x29,0xff

;--- GM 72 = Piccolo ---

patch_72:	.db	1		; transpose on
	.dw	patch_72hdr
	.db	41,113, 24 * 2
	.dw	frq_94
	.db	54,114, 53 * 2
	.dw	frq_118
	.db	60,115, 54 * 2
	.dw	frq_94
	.db	72,116, 60 * 2
	.dw	frq_288
	.db	255,117, 60 * 2
	.dw	frq_920
patch_72hdr:	.db 0x38,0xe0,2,0xff

;--- GM 73 = Flute ---

patch_73:	.db	1		; transpose on
	.dw	patch_72hdr
	.db	41,113, 24 * 2
	.dw	frq_94
	.db	54,114, 53 * 2
	.dw	frq_118
	.db	255,115, 54 * 2
	.dw	frq_94

;--- GM 74 = Recorder ---

patch_74:	.db	1		; transpose on
	.dw	patch_74hdr
	.db	255,189, 24 * 2
	.dw	frq_143
patch_74hdr:	.db 0x30,0xe0,1,0xff

;--- GM 75 = Pan Flute ---

patch_75:	.db	1		; transpose on
	.dw	patch_75hdr
	.db	255,119, 0 * 2
	.dw	frq_461
patch_75hdr:	.db 0x30,0xe0,3,0xff

;--- GM 76 = Blown Bottle ---

patch_76A:	.db	1		; transpose on
	.dw	patch_76Ahdr
	.db	255,119, 0 * 2
	.dw	frq_461
patch_76Ahdr:	.db 0x38,0xe0,1,0xff
patch_76B:	.db	1		; transpose on
	.dw	patch_76Bhdr
	.db	255,0x25, 1 + (12 + 2) * 2	  ; tone 293
	.dw	frq_21
patch_76Bhdr:	.db 0x0,0xff

;--- GM 77 = Shakuhachi ---

patch_77A:	.db	1		; transpose on
	.dw	patch_75hdr
	.db	255,171, 12 * 2
	.dw	frq_969
patch_77B:	.db	1		; transpose on
	.dw	patch_77Bhdr
	.db	255,118, 12 * 2
	.dw	frq_345
patch_77Bhdr:	.db 0x28,0xe0,2,0xff

;--- GM 78 = Whistle ---

patch_78:	.db	1		; transpose on
	.dw	patch_78hdr
	.db	255,170, 0 * 2
	.dw	frq_94
patch_78hdr:	.db 0x2c,0x98,0x70,0xff

;--- GM 79 = Ocarina ---

patch_79:	.db	1		; transpose on
	.dw	patch_79hdr
	.db	255,170, 0 * 2
	.dw	frq_94
patch_79hdr:	.db 0x29,0xe0,1,0xff

;--- GM 80 = Lead 1 (square) ---

patch_80A:	.db	1		; transpose on
	.dw	patch_80Ahdr
	.db	19,204, 60 * 2
	.dw	frq_341
	.db	31,205, 55 * 2
	.dw	frq_343
	.db	43,202, 55 * 2
	.dw	frq_343
	.db	55,207, 55 * 2
	.dw	frq_343
	.db	67,206, 55 * 2
	.dw	frq_343
	.db	255,203, 55 * 2
	.dw	frq_343
patch_80Ahdr:	.db 0x38,0x98,0xc6,0xff


patch_80B:	.db	1		; transpose on
	.dw	patch_80Bhdr
	.db	19,204, 60 * 2
	.dw	frq_335
	.db	31,205, 55 * 2
	.dw	frq_338
	.db	43,202, 55 * 2
	.dw	frq_338
	.db	55,207, 55 * 2
	.dw	frq_338
	.db	67,206, 55 * 2
	.dw	frq_338
	.db	255,203, 55 * 2
	.dw	frq_338
patch_80Bhdr:	.db 0x08,0x98,0xc6,0xff


;--- GM 81 = Lead 2 (sawtooth) ---

patch_81A:	.db	1		; transpose on
	.dw	patch_81Ahdr
	.db	16,0x18, 1 + 48 * 2	; tone 280
	.dw	frq_927
	.db	22,0x19, 1 + 64 * 2	; tone 281
	.dw	frq_358
	.db	28,0x1a, 1 + 58 * 2	; tone 282
	.dw	frq_925
	.db	34,0x1b, 1 + 64 * 2	; tone 283
	.dw	frq_358
	.db	40,0x1c, 1 + 58 * 2	; tone 284
	.dw	frq_936
	.db	46,0x1d, 1 + 64 * 2	; tone 285
	.dw	frq_364
	.db	52,0x1e, 1 + 58 * 2	; tone 286
	.dw	frq_949
	.db	58,0x1f, 1 + 64 * 2	; tone 287
	.dw	frq_364
	.db	64,0x20, 1 + 58 * 2	; tone 288
	.dw	frq_974
	.db	70,0x21, 1 + 64 * 2	; tone 289
	.dw	frq_340
	.db	79,0x22, 1 + 58 * 2	; tone 290
	.dw	frq_925
	.db	255,0x23, 1 + 55 * 2	 ; tone 291
	.dw	frq_925
patch_81Ahdr:	.db 0x30,0x98,0xf2,0xb0,0x22,0xc8,0x0a,0xff

patch_81B:	.db	1		; transpose on
	.dw	patch_81Ahdr
	.db	16,0x18, 1 + 48 * 2	; tone 280
	.dw	frq_918
	.db	22,0x19, 1 + 64 * 2	; tone 281
	.dw	frq_352
	.db	28,0x1a, 1 + 58 * 2	; tone 282
	.dw	frq_916
	.db	34,0x1b, 1 + 64 * 2	; tone 283
	.dw	frq_352
	.db	40,0x1c, 1 + 58 * 2	; tone 284
	.dw	frq_927
	.db	46,0x1d, 1 + 64 * 2	; tone 285
	.dw	frq_358
	.db	52,0x1e, 1 + 58 * 2	; tone 286
	.dw	frq_941
	.db	58,0x1f, 1 + 64 * 2	; tone 287
	.dw	frq_358
	.db	64,0x20, 1 + 58 * 2	; tone 288
	.dw	frq_966
	.db	70,0x21, 1 + 64 * 2	; tone 289
	.dw	frq_333
	.db	79,0x22, 1 + 58 * 2	; tone 290
	.dw	frq_916
	.db	255,0x23, 1 + 55 * 2	 ; tone 291
	.dw	frq_916

;--- GM 82 = Lead 3 (calliope) ---

patch_82A:	.db	1		; transpose on
	.dw	patch_82Ahdr
	.db	255,170, 0 * 2
	.dw	frq_94
patch_82Ahdr:	.db 0x28,0xe0,2,0xff
patch_82B:	.db	1		; transpose on
	.dw	patch_82Bhdr
	.db	255,118, 12 * 2
	.dw	frq_345
patch_82Bhdr:	.db 0x28,0x98,0xb0,0xe0,2,0xff

;--- GM 83 = Lead 4 (chiff) ---

patch_83A:	.db	1		; transpose on
	.dw	patch_83Ahdr
	.db	255,0x3a, 1 + 12 * 2	 ; tone 314
	.dw	frq_314
patch_83Ahdr:	.db 0x2a,0xe0,1,0xff
patch_83B:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	255,254, 12 * 2
	.dw	frq_311


;--- GM 84 = Lead 5 (charang) ---

patch_84A:	.db	1		; transpose on
	.dw	patch_84Ahdr
	.db	41,165, 36 * 2
	.dw	frq_231
	.db	255,166, 65 * 2
	.dw	frq_462
patch_84Ahdr:	.db 0x33,0xff

patch_84B:	.db	1		; transpose on
	.dw	patch_84Bhdr
	.db	16,0x18, 1 + 48 * 2	; tone 280
	.dw	frq_918
	.db	22,0x19, 1 + 64 * 2	; tone 281
	.dw	frq_352
	.db	28,0x1a, 1 + 58 * 2	; tone 282
	.dw	frq_916
	.db	34,0x1b, 1 + 64 * 2	; tone 283
	.dw	frq_352
	.db	40,0x1c, 1 + 58 * 2	; tone 284
	.dw	frq_927
	.db	46,0x1d, 1 + 64 * 2	; tone 285
	.dw	frq_358
	.db	52,0x1e, 1 + 58 * 2	; tone 286
	.dw	frq_941
	.db	58,0x1f, 1 + 64 * 2	; tone 287
	.dw	frq_358
	.db	64,0x20, 1 + 58 * 2	; tone 288
	.dw	frq_966
	.db	70,0x21, 1 + 64 * 2	; tone 289
	.dw	frq_333
	.db	79,0x22, 1 + 58 * 2	; tone 290
	.dw	frq_916
	.db	255,0x23, 1 + 55 * 2	 ; tone 291
	.dw	frq_916
patch_84Bhdr:	.db 0x2a,0x98,0xf2,0xb0,0x22,0xc8,0x0e,0xff


;--- GM 85 = Lead 6 (voice) ---

patch_85A:	.db	1		; transpose on
	.dw	patch_85Ahdr
	.db	255,170, 0 * 2
	.dw	frq_98
patch_85Ahdr:	.db 0x2e,0xff

patch_85B:	.db	1		; transpose on
	.dw	patch_85Bhdr
	.db	255,42, 12 * 2
	.dw	frq_204
patch_85Bhdr:	.db 0x29,0x98,0xf0,0xc8,0x0a,0xff


;--- GM 86 = Lead 7 (fifths) ---

patch_86A:	.db	1		; transpose on
	.dw	patch_86Ahdr
	.db	16,0x18, 1 + 48 * 2	; tone 280
	.dw	frq_918
	.db	22,0x19, 1 + 64 * 2	; tone 281
	.dw	frq_352
	.db	28,0x1a, 1 + 58 * 2	; tone 282
	.dw	frq_916
	.db	34,0x1b, 1 + 64 * 2	; tone 283
	.dw	frq_352
	.db	40,0x1c, 1 + 58 * 2	; tone 284
	.dw	frq_927
	.db	46,0x1d, 1 + 64 * 2	; tone 285
	.dw	frq_358
	.db	52,0x1e, 1 + 58 * 2	; tone 286
	.dw	frq_941
	.db	58,0x1f, 1 + 64 * 2	; tone 287
	.dw	frq_358
	.db	64,0x20, 1 + 58 * 2	; tone 288
	.dw	frq_966
	.db	70,0x21, 1 + 64 * 2	; tone 289
	.dw	frq_333
	.db	79,0x22, 1 + 58 * 2	; tone 290
	.dw	frq_916
	.db	255,0x23, 1 + 55 * 2	 ; tone 291
	.dw	frq_916
patch_86Ahdr:	.db 0x32,0x98,0xf5,0xff

patch_86B:	.db	1		; transpose on
	.dw	patch_86Bhdr
	.db	255,2, 12 * 2
	.dw	frq_929
patch_86Bhdr:	.db 0x31,0x98,0xb3,0xb0,0x22,0xc8,0x0b,0xff

;--- GM 87 = Lead 8 (bass + lead) ---

patch_87A:	.db	1		; transpose on
	.dw	patch_27hdr
	.db	255,0x17, 1 + 48 * 2	 ; tone 279
	.dw	frq_219


;--- GM 88 = Pad 1 (new age) ---
; B zachter dan A
patch_88A:	.db	1		; transpose on
	.dw	patch_88Ahdr
	.db	255,2, 12 * 2
	.dw	frq_927
patch_88Ahdr:	.db 0x24,0xff

patch_88B:	.db	1		; transpose on
	.dw	patch_88Bhdr
	.db	255,243, 12 * 2
	.dw	frq_721
patch_88Bhdr:	.db 0x23,0x98,0xa2,0xb0,0x14,0xc8,6,0xe0,1,0xff

;--- GM 89 = Pad 2 (warm) ---
; B zachter dan A
patch_89A:	.db	1		; transpose on
	.dw	patch_89Ahdr
	.db	255,78, 24 * 2
	.dw	frq_379
patch_89Ahdr:	.db 0x2a,0xe0,2,0xff

patch_89B:	.db	1		; transpose on
	.dw	patch_63Bhdr
	.db	255,41, 12 * 2
	.dw	frq_261


;--- GM 90 = Pad 3 (polysynth) ---

patch_90A:	.db	1		; transpose on
	.dw	patch_90Ahdr
	.db	16,0x18, 1 + 48 * 2	; tone 280
	.dw	frq_929
	.db	22,0x19, 1 + 64 * 2	; tone 281
	.dw	frq_360
	.db	28,0x1a, 1 + 58 * 2	; tone 282
	.dw	frq_927
	.db	34,0x1b, 1 + 64 * 2	; tone 283
	.dw	frq_360
	.db	40,0x1c, 1 + 58 * 2	; tone 284
	.dw	frq_939
	.db	46,0x1d, 1 + 64 * 2	; tone 285
	.dw	frq_366
	.db	52,0x1e, 1 + 58 * 2	; tone 286
	.dw	frq_951
	.db	58,0x1f, 1 + 64 * 2	; tone 287
	.dw	frq_366
	.db	64,0x20, 1 + 58 * 2	; tone 288
	.dw	frq_977
	.db	70,0x21, 1 + 64 * 2	; tone 289
	.dw	frq_341
	.db	79,0x22, 1 + 58 * 2	; tone 290
	.dw	frq_927
	.db	255,0x23, 1 + 55 * 2	 ; tone 291
	.dw	frq_927
patch_90Ahdr:	.db 0x23,0x98,0xf2,0xb0,0x22,0xc8,0x07,0xe0,1,0xff

patch_90B:	.db	1		; transpose on
	.dw	patch_90Bhdr
	.db	255,0x24, 1 + 12 * 2	 ; tone 292
	.dw	frq_550
patch_90Bhdr:	.db 0x22,0x98,0xe5,0xff

;--- GM 91 = Pad 4 (choir) ---

patch_91A:	.db	1		; transpose on
	.dw	patch_91Ahdr
	.db	35,24, 24 * 2
	.dw	frq_204
	.db	41,25, 47 * 2
	.dw	frq_815
	.db	48,26, 53 * 2
	.dw	frq_206
	.db	255,27, 48 * 2
	.dw	frq_356
patch_91Ahdr:	.db 0x25,0xc8,0x06,0xff

patch_91B:	.db	1		; transpose on
	.dw	patch_91Bhdr
	.db	255,42, 12 * 2
	.dw	frq_208
patch_91Bhdr:	.db 0x24,0x98,0x65,0xb0,0x21,0xff

;--- GM 92 = Pad 5 (bowed) ---

patch_92A:	.db	1		; transpose on
	.dw	patch_92Ahdr
	.db	255,2, 1 + 24 * 2 	 ; tone 257
	.dw	frq_88
patch_92Ahdr:	.db 0x19,0x98,0x44,0xe0,2,0xff
patch_92B:	.db	1		; transpose on
	.dw	patch_92Bhdr
	.db	255,170, 0 * 2
	.dw	frq_89
patch_92Bhdr:	.db 0x19,0x98,0x40,0xc8,6,0xff

;--- GM 93 = Pad 6 (metallic) ---

patch_93A:	.db	1		; transpose on
	.dw	patch_93Ahdr
	.db	26,12, 36 * 2
	.dw	frq_445
	.db	33,13, 50 * 2
	.dw	frq_450
	.db	48,14, 45 * 2
	.dw	frq_828
	.db	255,15, 48 * 2
	.dw	frq_828
patch_93Ahdr:	.db 0x0a,0xb0,0x03,0xc8,6,0xff

patch_93B:	.db	1		; transpose on
	.dw	patch_93Bhdr
	.db	255,63, 12 * 2
	.dw	frq_698
patch_93Bhdr:	.db 0x31,0x98,0x85,0xff

;--- GM 94 = Pad 7 (halo) ---
patch_94A:	.db	1		; transpose on
	.dw	patch_52hdr
	.db	255,0x24, 1 + 12 * 2	; 292
	.dw	frq_560

patch_94B:	.db	1		; transpose on
	.dw	patch_5Ahdr
	.db	255,42, 12 * 2
	.dw	frq_202


;--- GM 95 = Pad 8 (sweep) ---

patch_95A:	.db	1		; transpose on
	.dw	patch_95Ahdr
	.db	16,211, 48 * 2
	.dw	frq_920
	.db	22,218, 64 * 2
	.dw	frq_356
	.db	28,212, 58 * 2
	.dw	frq_920
	.db	34,219, 64 * 2
	.dw	frq_348
	.db	40,213, 58 * 2
	.dw	frq_937
	.db	46,220, 64 * 2
	.dw	frq_352
	.db	52,214, 58 * 2
	.dw	frq_926
	.db	58,221, 64 * 2
	.dw	frq_365
	.db	64,215, 58 * 2
	.dw	frq_974		; 975
	.db	70,222, 64 * 2
	.dw	frq_341
	.db	76,216, 58 * 2
	.dw	frq_926
	.db	255,217, 52 * 2
	.dw	frq_926
patch_95Ahdr:	.db 0x19,0x98,0x15,0xc8,6,0xff

patch_95B:	.db	1		; transpose on
	.dw	patch_95Bhdr
	.db	16,211, 48 * 2
	.dw	frq_909
	.db	22,218, 64 * 2
	.dw	frq_350
	.db	28,212, 58 * 2
	.dw	frq_909
	.db	34,219, 64 * 2
	.dw	frq_340
	.db	40,213, 58 * 2
	.dw	frq_927
	.db	46,220, 64 * 2
	.dw	frq_345
	.db	52,214, 58 * 2
	.dw	frq_916
	.db	58,221, 64 * 2
	.dw	frq_357
	.db	64,215, 58 * 2
	.dw	frq_964
	.db	70,222, 64 * 2
	.dw	frq_333
	.db	76,216, 58 * 2
	.dw	frq_926
	.db	255,217, 52 * 2
	.dw	frq_916

patch_95Bhdr:	.db 0x18,0x98,0x15,0xc8,6,0xff

;--- GM 96 = Rain ---

patch_96:	.db 0x		; transpose off
	.dw	patch_15Bhdr
	.db	255,0x43, 1 + (48 + 1) * 2	 ; tone 323
	.dw	frq_448


;--- GM 97 = Soundtrack ---

patch_97A:	.db	1		; transpose on
	.dw	patch_97Ahdr
	.db	255,2, 24 * 2
	.dw	frq_927
patch_97Ahdr:	.db 0x2a,0x98,0x60,0xff

patch_97B:	.db	1		; transpose on
	.dw	patch_97Bhdr
	.db	255,243, 0 * 2
	.dw	frq_704
patch_97Bhdr:	.db 0x2d,0x98,0x52,0xb0,0x14,0xc8,0x06,0xe0,2,0xff

;--- GM 98 = Crystal ---

patch_98A:	.db	1		; transpose on
	.dw	patch_98Ahdr
	.db	255,243, 0 * 2
	.dw	frq_143
patch_98Ahdr:	.db 0x33,0xff
patch_98B:	.db	1		; transpose on
	.dw	patch_98Bhdr
	.db	21,109, 36 * 2
	.dw	frq_824
	.db	31,110, 45 * 2
	.dw	frq_831
	.db	255,111, 43 * 2
	.dw	frq_843
patch_98Bhdr:	.db 0x3b,0x98,0x85,0xc8,6,0xff

;--- GM 99 = Atmosphere ---

patch_99A:	.db	1		; transpose on
	.dw	patch_99Ahdr
	.db	255,2, 24 * 2
	.dw	frq_927
patch_99Ahdr:	.db 0x32,0x98,0x73,0xb0,0x22,0xc8,6,0xe0,1,0xff

patch_99B:	.db	1		; transpose on
	.dw	patch_99Bhdr
	.db	24,179, 36 * 2
	.dw	frq_459
	.db	31,183, 48 * 2
	.dw	frq_555
	.db	37,181, 55 * 2
	.dw	frq_21
	.db	44,180, 49 * 2
	.dw	frq_459
	.db	255,182, 44 * 2
	.dw	frq_726
patch_99Bhdr:	.db 0x32,0xc8,7,0xe0,2,0xff

;--- GM 100 = Brightness ---

patch_100A:	.db	1		; transpose on
	.dw	patch_15Bhdr
	.db	255,0x37, 1 + 24 * 2	 ; tone 311
	.dw	frq_366
patch_100B:	.db	1		; transpose on
	.dw	patch_100Bhdr
	.db	255,42, 12 * 2
	.dw	frq_208
patch_100Bhdr:	.db 0x29,0xc8,6,0xff

;--- GM 102 = Echoes ---

patch_102A:	.db	1		; transpose on
	.dw	patch_102Ahdr
	.db	255,0x24, 1 + 24 * 2	 ; tone 292
	.dw	frq_552
patch_102Ahdr:	.db 0x2b,0x98,0xf5,0xb0,0x21,0xc8,6,0xff
patch_102B:	.db	1		; transpose on
	.dw	patch_102Bhdr
	.db	255,42, 12 * 2
	.dw	frq_211
patch_102Bhdr:	.db 0x2b,0x98,0x85,0xb0,0x23,0xc8,7,0xff

;--- GM 103 = FX 8 (sci-fi) ---

patch_103A:	.db	1		; transpose on
	.dw	patch_103Ahdr
	.db	26,12, 36 * 2
	.dw	frq_443
	.db	33,13, 50 * 2
	.dw	frq_448
	.db	48,14, 45 * 2
	.dw	frq_824
	.db	255,15, 48 * 2
	.dw	frq_824
patch_103Ahdr:	.db 0x2b,0x98,0x90,0xc8,6,0xe0,3,0xff

patch_103B:	.db	1		; transpose on
	.dw	patch_63Bhdr
	.db	255,2, 12 * 2
	.dw	frq_925


;--- GM 104 = Sitar ---

patch_104:	.db	1		; transpose on
	.dw	patch_104hdr
	.db	255,0xf, 1 + 12 * 2	 ; tone 271
	.dw	frq_847
patch_104hdr:	.db 0x32,0x98,0xf4,0xb0,0x15,0xc8,0x16,0xe0,1,0xff

;--- GM 105 = Banjo ---

patch_105:	.db	1		; transpose on
	.dw	patch_23Ahdr
	.db	29,19, 24 * 2
	.dw	frq_641
	.db	33,20, 53 * 2
	.dw	frq_203
	.db	37,18, 45 * 2
	.dw	frq_944
	.db	45,21, 49 * 2
	.dw	frq_288
	.db	53,23, 45 * 2
	.dw	frq_677
	.db	255,22, 53 * 2
	.dw	frq_45

;--- GM 106 = Shamisen ---

patch_106:	.db	1		; transpose on
	.dw	patch_2Bhdr
	.db	255,0xe, 1 + 12 * 2	 ; tone 270
	.dw	frq_97

;--- GM 107 = Koto ---

patch_107:	.db	1		; transpose on
	.dw	patch_0hdr
	.db	255,169, 12 * 2
	.dw	frq_556


;--- GM 108 = Kalimba ---

patch_108:	.db	1		; transpose on
	.dw	patch_23Ahdr
	.db	255,0x37, 1 + 12 * 2	 ; tone 311
	.dw	frq_366


;--- GM 109 = Bagpipes ---

patch_109A:	.db	1		; transpose on
	.dw	patch_109Ahdr
	.db	34,164, 48 * 2
	.dw	frq_1
	.db	255,168, 46 * 2
	.dw	frq_335
patch_109Ahdr:	.db 0x1c,0xff


patch_109B:	.db	1		; transpose on
	.dw	patch_109Bhdr
	.db	34,167, 48 * 2
	.dw	frq_0
	.db	255,168, 46 * 2
	.dw	frq_335
patch_109Bhdr:	.db 0x19,0xff

;--- GM 110 = Fiddle ---

patch_110:	.db	1		; transpose on
	.dw	patch_69hdr
	.db	35,0x5, 1 + 24 * 2	; tone 261
	.dw	frq_288
	.db	40,0x2, 1 + 59 * 2	; tone 258
	.dw	frq_69
	.db	42,0x6, 1 + 52 * 2	; tone 262
	.dw	frq_630
	.db	45,0x7, 1 + 54 * 2	; tone 263
	.dw	frq_556
	.db	48,0x8, 1 + 57 * 2	; tone 264
	.dw	frq_264
	.db	51,0xa, 1 + 48 * 2	; tone 266
	.dw	frq_920
	.db	53,0xb, 1 + 51 * 2	; tone 267
	.dw	frq_726
	.db	55,0xc, 1 + 53 * 2	; tone 268
	.dw	frq_579
	.db	58,0xd, 1 + 55 * 2	; tone 269
	.dw	frq_338
	.db	255,0x9, 1 + 58 * 2	 ; tone 265
	.dw	frq_143

;--- GM 111 = Shanai ---

patch_111:	.db	1		; transpose on
	.dw	patch_111hdr
	.db	255,65, 36 * 2
	.dw	frq_448
patch_111hdr:	.db 0x31,0xc8,9,0xff

;--- GM 112 = Tinkle Bell ---

patch_112A:	.db	1		; transpose on
	.dw	patch_112Ahdr
	.db	255,243, 0 * 2
	.dw	frq_721
patch_112Ahdr:	.db 0x3b,0x98,0xd6,0xff

patch_112B:	.db	1		; transpose on
	.dw	patch_17hdr
	.db	255,0x37, 1 + 36 * 2	; 311
	.dw	frq_214

;--- GM 113 = Agogo ---

patch_113:	.db 0x0		; transpose off
	.dw	patch_23Ahdr
	.db	255,11, 12 * 2
	.dw	frq_217


;--- GM 114 = Steel Drums ---

patch_114A:	.db 0x0		; transpose off
	.dw	patch_114Ahdr
	.db	255,254, 12 * 2
	.dw	frq_317
patch_114Ahdr:	.db 0x38,0xe0,1,0xff

patch_114B:	.db 0x0		; transpose off
	.dw	patch_114Bhdr
	.db	255,1, 1 + 36 * 2 	 ; tone 257
	.dw	frq_195
patch_114Bhdr:	.db 0x38,0x98,0xb0,0xb0,0x05,0xe0,1,0xff

;--- GM 115 = Woo.dblock ---

patch_115:	.db 0x0		; transpose off
	.dw	patch_115hdr
	.db	255,44, 24 * 2
	.dw	frq_355
patch_115hdr:	.db 0x0,0xff

;--- GM 116 = Taiko Drum ---

patch_116:	.db 0x		; transpose off
	.dw	patch_23Ahdr
	.db	255,62, 12 * 2
	.dw	frq_217

;--- GM 117 = Melodic Tom ---

patch_117:	.db 0x0		; transpose off
	.dw	patch_117hdr
	.db	255,199, 36 * 2
	.dw	frq_616
patch_117hdr:	.db 0x38,0xc8,1,0xff

;--- GM 118 = Synth Drum ---

patch_118:	.db 0x0		; transpose off
	.dw	patch_23Ahdr
	.db	255,38, 24 * 2
	.dw	frq_27

;--- GM 119 = Reverse Cymbal ---

patch_119:	.db 0x0		; transpose off
	.dw	patch_119hdr
	.db	255,49, 36 * 2
	.dw	frq_143
patch_119hdr:	.db 0x38,0xc8,9,0xff

;--- GM 120 = Gtr Fret Noise ---

patch_120:	.db 0x0		; transpose off
	.dw	patch_23Ahdr
	.db	255,0x38, 1 + 24 * 2	 ; tone 312
	.dw	frq_354

;--- GM 121 = Breath Noise ---

patch_121:	.db 0x0		; transpose off
	.dw	patch_23Ahdr
	.db	255,0x25, 1 + 24 * 2	 ; tone 293
	.dw	frq_733

;--- GM 122 = Sea Shore ---

patch_122A:	.db 0x0		; transpose off
	.dw	patch_122Ahdr
	.db	255,8, 36 * 2
	.dw	frq_264
patch_122Ahdr:	.db 0x0,0xc8,4,0xff
patch_122B:	.db 0x0		; transpose off
	.dw	patch_122Bhdr
	.db	255,55, 48 * 2
	.dw	frq_203
patch_122Bhdr:	.db 0x0,0x98,0x20,0xb0,4,0xc8,5,0xe0,0x0,0xff


;--- GM 124 = Telephone / Bird ---

patch_124:	.db 0x0		; transpose off
	.dw	patch_115hdr
	.db	48, 3, 36 * 2
	.dw	frq_27
	.db	255, 9, 36 * 2
    .dw frq_203


;--- GM 125 = Helicopter ---

patch_125A:	.db 0x0		; transpose off
	.dw	patch_125Ahdr
	.db	255, 1, 24 * 2
	.dw	frq_560
patch_125Ahdr:	.db	8,0xc8,7,0xff
patch_125B:	.db 0x0		; transpose off
	.dw	patch_125Bhdr
	.db	255, 55, 36 * 2
	.dw	frq_462
patch_125Bhdr:	.db 0x,0xc8,7,0xe0,0x,0xff


;--- GM 126 = Applause ---

patch_126A:	.db 0x0		; transpose off
	.dw	patch_126Ahdr
	.db	255, 54, 24 * 2
	.dw	frq_216
patch_126Ahdr:	.db 0xf,0x98,0x70,0xff
patch_126B:	.db 0x0		; transpose off
	.dw	patch_126Bhdr
	.db	255, 54, 24 * 2
	.dw	frq_462
patch_126Bhdr:	.db 0x7,0x98,0x70,0xff

;--- GM 127 = Gunshot / Snare ---

patch_127:	.db 0x0		; transpose off
	.dw	patch_127hdr
	.db	48,0x39, 1 + 24 * 2        ; tone 313
	.dw	frq_203
;	 .db	 255, 59, 1 + 24 * 2
	.db	255, 120, 0 + 24 * 2
	.dw	frq_203
patch_127hdr:   .db    038,0xc8,3,0xff

;--- Drum patch 1 ---

drum_1:	.db 0x0		; transpose off
	.dw	drum_1hdr
	.db	48,17, 24 * 2		; tone 17
	.dw	frq_264
	.db	255,191, 36 * 2		; tone 191
	.dw	frq_264
drum_1hdr:	.db 0x0,0x98,0xf4,0xb0,0xfe,0xc8,6,0xff

;--- Drum patch 2 ---

drum_2:	.db 0x0		; transpose off
	.dw	drum_2hdr
	.db	48,209, 24 * 2		; tone 209
	.dw	frq_908
	.db	255,197, 24 * 2		; tone 197
	.dw	frq_908
drum_2hdr:      .db    0,0x98,0xf5,0xb0,0xf2,0xc8,6,0xff


;--- Drum patch 3 ---

drum_3:	.db 0x0		; transpose off
	.dw	drum_3hdr
	.db	255,10, 24 * 2		 ; tone 10
	.dw	frq_654
drum_3hdr:	.db 0x0,0x98,0xf4,0xb0,0xf2,0xc8,6,0xff

;--- Drum patch 4 ---

drum_4:	.db 0x0		; transpose off
	.dw	drum_3hdr
	.db	255,28, 24 * 2		 ; tone 28
	.dw	frq_187

;--- Drum patch 5 ---

drum_5:	.db 0x0		; transpose off
	.dw	drum_3hdr
	.db	255,31, 24 * 2		 ; tone 31
	.dw	frq_908

;--- Drum patch 6 ---

drum_6:	.db 0x0		; transpose off
	.dw	drum_3hdr
	.db	255,156, 24 * 2		 ; tone 156
	.dw	frq_27

;--- Drum patch 7 ---

drum_7:	.db 0x0		; transpose off
	.dw	drum_3hdr
	.db	255,49, 24 * 2		 ; tone 49
	.dw	frq_24

;--- Drum patch 8 ---

drum_8:	.db 0x0		; transpose off
	.dw	drum_8hdr
	.db	255,157, 24 * 2		 ; tone 157
	.dw	frq_737
drum_8hdr:	.db 0x0,0xb0,0x27,0xc8,6,0xff

gmdrm_hdr1:	.db 0x0,0x98,0xf4,0xb0,0xf2,0xc8,6,0xff
gmdrm_hdr2:	.db 0x0,0x98,0xf5,0xb0,0xf2,0xc8,6,0xff
gmdrm_hdr3:	.db 0x0,0x98,0xf2,0xb0,0xf2,0xc8,6,0xff
gmdrm_hdr4:	.db 0x0,0x98,0xf1,0xb0,0xf2,0xc8,6,0xff
gmdrm_hdr5:	.db 0x0,0x98,0xf4,0xb0,0x27,0xc8,6,0xff
gmdrm_hdr6:	.db 0x0,0x98,0xf4,0xb0,0xf2,0xc8,7,0xff
gmdrm_hdr7:	.db 0x0,0x98,0xf6,0xb0,0xff,0xc8,6,0xff
gmdrm_hdr8:	.db 0x0,0x98,0xf4,0xb0,0xf2,0xc8,6,0xff
gmdrm_hdr9:	.db 0x0,0x98,0xf4,0xb0,0xf2,0xc8,4,0xff

;--- Midi Drum patch ---

drum_midi:	.db 0x0		; transpose off
	.dw	gmdrm_hdr1
	.db	36,16, (48 - 35) * 2	; tone 16
	.dw	frq_311

gmdrm_c4:	.db	17
	.db 0x18,0xf2
	.dw	gmdrm_hdr1
gmdrm_cp4:	.db	210
	.db 0xf6,0x
	.dw	gmdrm_hdr1
gmdrm_d4:	.db	209
	.db 0xf4,0xf6
	.dw	gmdrm_hdr2
gmdrm_dp4:	.db	10
	.db 0x2a,0xf5
	.dw	gmdrm_hdr1
gmdrm_e4:	.db	209
	.db 0x38,0x03
	.dw	gmdrm_hdr3
gmdrm_f4:	.db	200
	.db 0x60,0x
	.dw	gmdrm_hdr1
gmdrm_fp4:	.db	121
	.db 0x5c,1
	.dw	gmdrm_hdr1
gmdrm_g4:	.db	200
	.db 0xa8,2
	.dw	gmdrm_hdr1
gmdrm_gp4:	.db	123
	.db 0x34,5
	.dw	gmdrm_hdr1
gmdrm_a4:	.db	200
	.db 0x46,4
	.dw	gmdrm_hdr1
gmdrm_ap4:	.db	122
	.db 0x34,5
	.dw	gmdrm_hdr4
gmdrm_b4:	.db	199
	.db 0x0a,3
	.dw	gmdrm_hdr1

gmdrm_c5:	.db	199
	.db 0xf4,4
	.dw	gmdrm_hdr1
gmdrm_cp5:	.db	49
	.db 0x32,0x
	.dw	gmdrm_hdr1
gmdrm_d5:	.db	199
	.db 0x90,0x10
	.dw	gmdrm_hdr1
gmdrm_dp5:	.db	46
	.db 0xce,1
	.dw	gmdrm_hdr1
gmdrm_e5:	.db	122
	.db 0x56,0xe1
	.dw	gmdrm_hdr1
gmdrm_f5:	.db	33
	.db 0x3c,1
	.dw	gmdrm_hdr1
gmdrm_fp5:	.db	37
	.db 0xba,0x
	.dw	gmdrm_hdr3
gmdrm_g5:	.db	49
	.db 0xca,5
	.dw	gmdrm_hdr2
gmdrm_gp5:	.db	29
	.db 0xfe,0xf6
	.dw	gmdrm_hdr1
gmdrm_a5:	.db	49
	.db 0x70,3
	.dw	gmdrm_hdr1
gmdrm_ap5:	.db	157
	.db 0xca,0xf5
	.dw	gmdrm_hdr5
gmdrm_b5:	.db	46
	.db 0x74,5
	.dw	gmdrm_hdr6

gmdrm_c6:	.db	28
	.db 0x76,1
	.dw	gmdrm_hdr1
gmdrm_cp6:	.db	28
	.db 0x32,0x
	.dw	gmdrm_hdr1
gmdrm_d6:	.db	30
	.db 0xda,0xf4
	.dw	gmdrm_hdr1
gmdrm_dp6:	.db	31
	.db 0xfe,0xf6
	.dw	gmdrm_hdr1
gmdrm_e6:	.db	31
	.db 0xfc,0xf2
	.dw	gmdrm_hdr1
gmdrm_f6:	.db	156
	.db 0x3e,0x
	.dw	gmdrm_hdr1
gmdrm_fp6:	.db	156
	.db 0xd0,0xf4
	.dw	gmdrm_hdr1
gmdrm_g6:	.db	11
	.db 0x16,0x
	.dw	gmdrm_hdr1
gmdrm_gp6:	.db	11
	.db 0x3c,0xf4
	.dw	gmdrm_hdr1
gmdrm_a6:	.db	47
	.db 0x50,3
	.dw	gmdrm_hdr1
gmdrm_ap6:	.db	48
	.db 0x4a,0x
	.dw	gmdrm_hdr1
gmdrm_b6:	.db	45
	.db 0x38,0xf3
	.dw	gmdrm_hdr1

gmdrm_c7:	.db 0x44
	.db 0x53,0xf2
	.dw	gmdrm_hdr7
gmdrm_cp7:	.db	36
	.db 0x74,5
	.dw	gmdrm_hdr8
gmdrm_d7:	.db	36
	.db 0x48,0xf6
	.dw	gmdrm_hdr1
gmdrm_dp7:	.db	32
	.db 0x50,0xf5
	.dw	gmdrm_hdr1
gmdrm_e7:	.db	44
	.db 0x28,0xf2
	.dw	gmdrm_hdr1
gmdrm_f7:	.db	44
	.db 0x5c,0xe7
	.dw	gmdrm_hdr1
gmdrm_fp7:	.db	34
	.db 0x22,0x10
	.dw	gmdrm_hdr1
gmdrm_g7:	.db	35
	.db 0xc0,0xf6
	.dw	gmdrm_hdr1
gmdrm_gp7:	.db	50
	.db 0x50,0x
	.dw	gmdrm_hdr1
gmdrm_a7:	.db	50
	.db 0x54,0x
	.dw	gmdrm_hdr9
gmdrm_ap7:	.db	191
	.db 0x18,0xf2
                .dw    gmdrm_hdr1
gmdrm_b7:	.db	197
	.db 0x18,0xf7
                .dw    gmdrm_hdr1
gmdrm_c8:	.db	197
	.db 0x38,0x03
	.dw	gmdrm_hdr1
gmdrm_cp8:	.db	208
	.db 0x18,0xf2
                .dw    gmdrm_hdr1
gmdrm_d8:	.db	120
	.db 0x18,0xf2
                .dw    gmdrm_hdr1



frq_0:     .dw  0,61,125,194,266,343,424,510,601,698,801,909
frq_1:     .dw  1,62,127,195,268,345,426,512,604,700,803,912
frq_8:     .dw  8,69,134,203,276,353,435,522,614,711,814,924
frq_9:     .dw  9,70,135,204,277,355,437,524,616,713,816,926
frq_10:    .dw  10,71,136,205,278,356,438,525,617,714,818,927
frq_15:    .dw  15,77,143,212,286,364,446,533,626,724,828,938
frq_20:    .dw  20,82,148,217,291,369,452,540,633,731,836,946
frq_21:    .dw  21,83,148,218,292,370,453,542,635,733,838,949
frq_22:    .dw  22,84,150,219,293,372,455,542,636,734,839,950

frq_24:    .dw  24,86,152,222,296,375,457,545,639,737,842,953
frq_26:    .dw  26,88,154,224,299,377,461,549,642,741,846,958
frq_27:    .dw  27,89,155,225,300,378,462,550,644,743,848,959
frq_45:    .dw  45,109,176,248,324,403,488,578,673,774,881,995
frq_46:    .dw  46,110,177,248,324,404,489,579,674,775,882,996

frq_69:    .dw  69,134,203,276,353,435,522,614,711,815,924,8+2048
frq_70:    .dw  70,135,204,277,354,436,523,615,712,815,925,8+2048
frq_73:    .dw  73,139,208,281,359,441,528,620,718,822,932,12+2048
frq_75:    .dw  75,141,210,283,361,444,531,623,721,825,935,14+2048
frq_79:    .dw  79,145,214,288,366,449,536,629,727,831,942,17+2048

frq_82:    .dw  82,147,217,291,369,452,540,633,731,835,946,20+2048
frq_84:    .dw  84,150,219,293,372,455,543,636,734,839,950,22+2048
frq_88:    .dw  88,154,224,298,377,460,548,642,741,846,957,25+2048
frq_89:    .dw  89,155,225,299,378,461,550,643,742,847,959,26+2048
frq_91:    .dw  91,157,228,302,381,465,553,647,746,851,963,29+2048

frq_94:    .dw  94,161,231,306,385,469,557,651,751,857,968,31+2048

frq_96:    .dw  96,162,233,308,387,471,560,654,753,859,971,33+2048
frq_97:    .dw  97,164,235,309,389,473,562,656,756,862,974,34+2048
frq_98:    .dw  98,165,236,311,390,474,563,657,757,863,976,35+2048
frq_113:   .dw  113,180,252,328,408,493,583,679,780,887,1001,49+2048
frq_118:   .dw  118,186,257,334,415,501,591,687,789,897,1011,54+2048

frq_135:   .dw  135,204,277,355,437,524,616,713,816,926,9+2048,70+2048
frq_141:   .dw  141,210,283,361,443,530,623,721,825,935,13+2048,75+2048
frq_143:   .dw  143,212,286,363,446,533,626,724,828,938,15+2048,77+2048

frq_155:   .dw  155,225,299,376,461,549,643,742,847,959,26+2048,89+2048
frq_156:   .dw  156,226,300,379,462,551,644,744,849,960,27+2048,89+2048

frq_167:   .dw  167,237,312,392,476,565,660,760,866,978,37+2048,100+2048

frq_184:   .dw  184,256,332,413,499,589,685,787,894,1008,53+2048,117+2048

frq_187:   .dw  187,259,336,417,502,593,689,791,899,1013,55+2048,119+2048
frq_188:   .dw  188,260,336,417,503,593,690,792,899,1014,56+2048,120+2048

frq_191:   .dw  191,263,340,421,507,598,694,796,905,1019,58+2048,123+2048
frq_192:   .dw  192,264,341,422,508,599,696,798,906,1021,59+2048,124+2048
frq_195:   .dw  195,267,344,425,512,603,700,802,911,1+2048,62+2048,126+2048

frq_202:   .dw  202,275,352,434,521,613,710,813,922,7+2048,68+2048,133+2048
frq_203:   .dw  203,276,353,435,522,614,711,815,924,8+2048,69+2048,134+2048

frq_204:   .dw  204,277,354,436,523,615,712,815,925,8+2048,70+2048,135+2048
frq_206:   .dw  206,279,356,438,525,618,715,819,928,10+2048,72+2048,137+2048
frq_208:   .dw  208,281,359,441,528,621,718,822,932,12+2048,74+2048,139+2048
frq_211:   .dw  211,284,362,445,532,624,722,826,936,14+2048,76+2048,142+2048
frq_214:   .dw  214,288,366,449,536,629,727,831,942,17+2048,79+2048,145+2048
frq_215:   .dw  215,289,367,450,538,630,729,833,943,18+2048,80+2048,146+2048
frq_216:   .dw  216,289,368,450,538,631,729,834,944,19+2048,80+2048,146+2048
frq_217:   .dw  217,291,369,452,540,633,731,835,946,20+2048,82+2048,147+2048
frq_219:   .dw  219,293,372,455,543,636,734,839,950,22+2048,84+2048,150+2048
frq_220:   .dw  220,294,373,456,544,637,736,841,951,22+2048,85+2048,151+2048
frq_221:   .dw  221,296,374,457,545,639,737,842,953,23+2048,86+2048,152+2048
frq_228:   .dw  228,302,381,465,553,647,747,852,963,29+2048,91+2048,158+2048
frq_231:   .dw  231,305,384,468,557,651,750,856,967,31+2048,94+2048,160+2048
frq_242:   .dw  242,317,397,481,571,666,766,873,985,40+2048,104+2048,171+2048
frq_254:   .dw  254,330,410,495,586,681,783,890,1004,50+2048,114+2048,182+2048
frq_261:   .dw  261,337,418,504,595,691,793,901,1016,56+2048,121+2048,189+2048
frq_264:   .dw  264,341,422,508,599,695,797,906,1020,59+2048,123+2048,192+2048
frq_273:   .dw  273,350,432,519,610,708,810,920,6+2048,67+2048,132+2048,200+2048
frq_274:   .dw  274,351,433,519,611,708,811,920,6+2048,67+2048,132+2048,201+2048
frq_282:   .dw  282,360,442,529,621,719,823,933,13+2048,74+2048,139+2048,209+2048
frq_286:   .dw  286,363,446,533,626,724,828,938,15+2048,77+2048,143+2048,212+2048
frq_287:   .dw  287,365,447,535,627,726,830,940,16+2048,78+2048,144+2048,213+2048
frq_288:   .dw  288,366,449,536,629,727,831,942,17+2048,79+2048,145+2048,214+2048
frq_290:   .dw  290,368,451,539,632,730,835,945,19+2048,81+2048,147+2048,216+2048
frq_291:   .dw  291,370,453,540,633,731,836,946,20+2048,82+2048,148+2048,217+2048
frq_299:   .dw  299,378,461,550,643,742,848,959,26+2048,89+2048,155+2048,225+2048
frq_311:   .dw  311,391,475,564,658,758,864,977,36+2048,99+2048,166+2048,236+2048
frq_314:   .dw  314,394,478,567,662,762,868,981,38+2048,101+2048,168+2048,239+2048
frq_317:   .dw  317,397,481,571,666,766,873,986,41+2048,104+2048,171+2048,242+2048
frq_333:   .dw  333,414,500,590,686,788,896,1010,53+2048,117+2048,185+2048,257+2048
frq_335:   .dw  335,416,502,593,689,791,899,1013,55+2048,119+2048,187+2048,259+2048
frq_338:   .dw  338,419,505,596,692,794,902,1016,56+2048,121+2048,189+2048,261+2048
frq_340:   .dw  340,421,507,598,694,796,905,1019,58+2048,123+2048,191+2048,263+2048
frq_341:   .dw  341,422,508,599,696,798,906,1021,59+2048,124+2048,192+2048,264+2048
frq_342:   .dw  342,423,509,600,697,799,907,1022,60+2048,125+2048,193+2048,265+2048
frq_343:   .dw  343,425,511,602,699,801,910,0+2048,61+2048,126+2048,195+2048,267+2048
frq_345:   .dw  345,427,513,604,701,804,913,2+2048,63+2048,127+2048,196+2048,268+2048
frq_348:   .dw  348,429,516,607,704,806,915,3+2048,64+2048,129+2048,198+2048,271+2048
frq_350:   .dw  350,430,518,609,705,808,917,4+2048,65+2048,130+2048,199+2048,272+2048
frq_352:   .dw  352,434,521,613,710,813,922,7+2048,68+2048,133+2048,202+2048,275+2048
frq_354:   .dw  354,436,523,615,712,815,925,8+2048,70+2048,135+2048,204+2048,277+2048
frq_355:   .dw  355,437,524,616,713,816,926,9+2048,70+2048,135+2048,204+2048,277+2048
frq_356:   .dw  356,438,525,617,714,818,927,10+2048,71+2048,136+2048,205+2048,278+2048
frq_357:   .dw  357,439,526,618,716,820,929,11+2048,72+2048,137+2048,206+2048,279+2048
frq_358:   .dw  358,441,528,620,718,821,931,12+2048,73+2048,138+2048,207+2048,281+2048
frq_360:   .dw  360,443,530,622,720,824,934,13+2048,75+2048,140+2048,209+2048,283+2048
frq_361:   .dw  361,443,531,623,722,825,935,14+2048,76+2048,141+2048,211+2048,284+2048
frq_363:   .dw  363,445,533,625,723,827,937,15+2048,77+2048,142+2048,211+2048,285+2048
frq_364:   .dw  364,447,534,627,725,829,939,16+2048,78+2048,144+2048,213+2048,286+2048
frq_365:   .dw  365,447,535,627,726,830,940,16+2048,78+2048,144+2048,213+2048,287+2048
frq_366:   .dw  366,449,536,629,727,831,942,17+2048,79+2048,145+2048,214+2048,288+2048
frq_367:   .dw  367,450,537,630,729,833,943,18+2048,80+2048,146+2048,216+2048,289+2048
frq_369:   .dw  369,452,540,633,731,835,946,20+2048,82+2048,147+2048,217+2048,291+2048
frq_370:   .dw  370,453,541,634,732,837,947,20+2048,82+2048,148+2048,218+2048,292+2048
frq_373:   .dw  373,457,545,638,737,841,952,23+2048,85+2048,151+2048,221+2048,295+2048
frq_378:   .dw  378,462,550,644,743,848,959,27+2048,89+2048,155+2048,225+2048,300+2048
frq_379:   .dw  378,463,551,645,744,849,960,27+2048,90+2048,156+2048,226+2048,300+2048
frq_381:   .dw  381,465,553,647,747,852,963,29+2048,91+2048,158+2048,228+2048,302+2048
frq_385:   .dw  385,469,558,652,752,857,969,32+2048,95+2048,161+2048,232+2048,306+2048
frq_433:   .dw  433,520,612,709,812,921,7+2048,68+2048,133+2048,202+2048,274+2048,352+2048
frq_434:   .dw  434,521,613,710,813,922,7+2048,68+2048,133+2048,202+2048,275+2048,352+2048
frq_436:   .dw  436,523,615,712,815,925,8+2048,70+2048,135+2048,204+2048,277+2048,354+2048
frq_442:   .dw  442,529,621,719,823,933,12+2048,74+2048,139+2048,209+2048,282+2048,360+2048
frq_443:   .dw  443,530,622,720,824,934,13+2048,75+2048,140+2048,209+2048,283+2048,360+2048
frq_445:   .dw  445,532,624,723,826,936,14+2048,76+2048,142+2048,211+2048,284+2048,362+2048
frq_448:   .dw  448,535,628,726,830,941,17+2048,79+2048,144+2048,214+2048,287+2048,365+2048
frq_450:   .dw  450,538,631,730,834,944,19+2048,81+2048,146+2048,216+2048,290+2048,368+2048
frq_455:   .dw  455,543,637,735,840,951,22+2048,84+2048,150+2048,220+2048,294+2048,372+2048
frq_459:   .dw  459,547,640,739,844,955,24+2048,87+2048,153+2048,223+2048,297+2048,375+2048
frq_460:   .dw  460,548,641,740,845,957,25+2048,88+2048,154+2048,224+2048,298+2048,376+2048
frq_461:   .dw  461,549,642,741,846,958,26+2048,88+2048,154+2048,224+2048,299+2048,377+2048
frq_462:   .dw  462,550,644,743,848,959,27+2048,89+2048,155+2048,225+2048,300+2048,378+2048
frq_480:   .dw  480,569,664,764,870,983,39+2048,102+2048,169+2048,240+2048,316+2048,395+2048
frq_507:   .dw  507,598,694,796,905,1019,58+2048,123+2048,191+2048,263+2048,340+2048,421+2048
frq_508:   .dw  508,599,695,797,905,1019,58+2048,123+2048,191+2048,264+2048,341+2048,422+2048
frq_519:   .dw  519,611,708,811,920,6+2048,67+2048,132+2048,201+2048,274+2048,351+2048,433+2048
frq_531:   .dw  531,623,721,825,935,14+2048,76+2048,141+2048,210+2048,284+2048,362+2048,444+2048
frq_532:   .dw  532,624,722,826,936,14+2048,76+2048,141+2048,211+2048,284+2048,362+2048,445+2048
frq_538:   .dw  538,631,730,834,944,19+2048,81+2048,146+2048,216+2048,290+2048,368+2048,451+2048
frq_545:   .dw  545,639,737,842,953,23+2048,86+2048,152+2048,221+2048,296+2048,374+2048,457+2048
frq_548:   .dw  548,642,741,846,957,25+2048,88+2048,154+2048,224+2048,298+2048,377+2048,460+2048
frq_550:   .dw  550,643,742,848,959,26+2048,89+2048,155+2048,225+2048,299+2048,378+2048,461+2048
frq_552:   .dw  552,645,745,850,961,28+2048,90+2048,156+2048,227+2048,301+2048,380+2048,463+2048
frq_555:   .dw  555,648,748,853,965,30+2048,92+2048,159+2048,229+2048,304+2048,383+2048,467+2048
frq_556:   .dw  556,650,749,855,967,30+2048,93+2048,160+2048,231+2048,305+2048,384+2048,467+2048
frq_557:   .dw  557,651,750,856,967,31+2048,94+2048,160+2048,231+2048,305+2048,384+2048,468+2048
frq_558:   .dw  558,652,752,857,969,32+2048,95+2048,161+2048,232+2048,306+2048,385+2048,469+2048
frq_559:   .dw  559,653,753,858,970,32+2048,95+2048,162+2048,232+2048,307+2048,386+2048,470+2048
frq_560:   .dw  560,654,753,859,971,33+2048,96+2048,162+2048,233+2048,308+2048,387+2048,471+2048
frq_579:   .dw  579,674,775,882,996,46+2048,110+2048,177+2048,248+2048,324+2048,404+2048,489+2048
frq_616:   .dw  616,713,816,926,9+2048,70+2048,135+2048,204+2048,277+2048,355+2048,437+2048,524+2048
frq_627:   .dw  627,725,829,939,16+2048,78+2048,143+2048,213+2048,286+2048,364+2048,447+2048,534+2048
frq_630:   .dw  630,729,833,943,18+2048,80+2048,146+2048,215+2048,289+2048,367+2048,450+2048,538+2048
frq_639:   .dw  639,738,843,954,24+2048,86+2048,152+2048,222+2048,296+2048,375+2048,458+2048,546+2048
frq_641:   .dw  641,740,845,957,25+2048,88+2048,154+2048,224+2048,298+2048,376+2048,460+2048,548+2048
frq_645:   .dw  645,744,849,960,27+2048,90+2048,156+2048,226+2048,300+2048,380+2048,463+2048,552+2048
frq_650:   .dw  650,749,855,967,30+2048,93+2048,160+2048,230+2048,305+2048,384+2048,467+2048,556+2048
frq_653:   .dw  653,753,858,970,32+2048,95+2048,162+2048,232+2048,307+2048,386+2048,470+2048,559+2048
frq_654:   .dw  654,754,859,971,33+2048,96+2048,163+2048,233+2048,308+2048,387+2048,471+2048,560+2048
frq_677:   .dw  677,778,886,999,48+2048,112+2048,179+2048,251+2048,326+2048,407+2048,492+2048,582+2048
frq_679:   .dw  679,781,888,1002,49+2048,113+2048,180+2048,252+2048,328+2048,408+2048,494+2048,584+2048
frq_698:   .dw  698,801,909,0+2048,61+2048,125+2048,194+2048,266+2048,343+2048,424+2048,510+2048,601+2048
frq_701:   .dw  701,805,913,2+2048,63+2048,127+2048,196+2048,268+2048,345+2048,427+2048,513+2048,604+2048
frq_704:   .dw  704,807,916,4+2048,65+2048,130+2048,198+2048,271+2048,348+2048,429+2048,516+2048,607+2048
frq_708:   .dw  708,810,920,6+2048,67+2048,132+2048,200+2048,273+2048,350+2048,432+2048,519+2048,610+2048
frq_715:   .dw  715,819,928,10+2048,72+2048,137+2048,206+2048,279+2048,356+2048,438+2048,525+2048,618+2048
frq_721:   .dw  721,825,935,13+2048,75+2048,141+2048,210+2048,283+2048,361+2048,443+2048,530+2048,623+2048
frq_726:   .dw  726,830,941,17+2048,79+2048,144+2048,214+2048,287+2048,365+2048,448+2048,535+2048,628+2048
frq_727:   .dw  727,831,942,18+2048,80+2048,145+2048,215+2048,288+2048,367+2048,449+2048,536+2048,629+2048
frq_733:   .dw  733,838,949,21+2048,83+2048,149+2048,219+2048,293+2048,371+2048,454+2048,542+2048,635+2048
frq_737:   .dw  737,842,953,23+2048,86+2048,152+2048,221+2048,296+2048,374+2048,457+2048,545+2048,639+2048
frq_750:   .dw  750,855,967,31+2048,93+2048,160+2048,230+2048,305+2048,384+2048,467+2048,556+2048,650+2048
frq_775:   .dw  775,882,996,46+2048,110+2048,177+2048,248+2048,324+2048,404+2048,489+2048,579+2048,674+2048
frq_809:   .dw  809,918,5+2048,66+2048,131+2048,199+2048,272+2048,349+2048,431+2048,517+2048,609+2048,706+2048
frq_810:   .dw  810,919,5+2048,66+2048,131+2048,200+2048,273+2048,350+2048,431+2048,518+2048,610+2048,707+2048
frq_811:   .dw  811,920,6+2048,67+2048,132+2048,201+2048,274+2048,351+2048,433+2048,519+2048,611+2048,708+2048
frq_815:   .dw  815,925,8+2048,70+2048,135+2048,204+2048,277+2048,354+2048,436+2048,523+2048,615+2048,712+2048
frq_820:   .dw  820,929,11+2048,72+2048,137+2048,206+2048,280+2048,357+2048,439+2048,526+2048,618+2048,716+2048
frq_822:   .dw  822,932,12+2048,73+2048,139+2048,208+2048,281+2048,359+2048,441+2048,528+2048,620+2048,718+2048
frq_823:   .dw  823,933,13+2048,74+2048,139+2048,209+2048,282+2048,360+2048,442+2048,529+2048,621+2048,719+2048
frq_824:   .dw  824,934,13+2048,75+2048,140+2048,209+2048,283+2048,360+2048,443+2048,530+2048,622+2048,720+2048
frq_828:   .dw  828,938,15+2048,77+2048,143+2048,212+2048,286+2048,363+2048,446+2048,533+2048,626+2048,724+2048
frq_831:   .dw  831,942,17+2048,79+2048,145+2048,214+2048,288+2048,366+2048,449+2048,536+2048,629+2048,727+2048
frq_832:   .dw  832,942,18+2048,80+2048,145+2048,215+2048,288+2048,367+2048,449+2048,537+2048,630+2048,728+2048
frq_833:   .dw  833,943,18+2048,80+2048,146+2048,215+2048,289+2048,367+2048,450+2048,538+2048,630+2048,729+2048
frq_835:   .dw  835,946,20+2048,82+2048,147+2048,217+2048,291+2048,369+2048,452+2048,540+2048,633+2048,731+2048
frq_842:   .dw  842,953,23+2048,86+2048,152+2048,222+2048,296+2048,374+2048,457+2048,545+2048,639+2048,737+2048
frq_843:   .dw  843,954,24+2048,86+2048,152+2048,222+2048,296+2048,375+2048,458+2048,546+2048,639+2048,738+2048
frq_844:   .dw  844,955,24+2048,87+2048,153+2048,223+2048,297+2048,375+2048,459+2048,547+2048,640+2048,739+2048
frq_847:   .dw  847,959,26+2048,89+2048,155+2048,225+2048,299+2048,378+2048,461+2048,550+2048,643+2048,742+2048
frq_852:   .dw  852,964,29+2048,92+2048,158+2048,228+2048,303+2048,382+2048,465+2048,554+2048,648+2048,747+2048
frq_874:   .dw  874,986,41+2048,104+2048,171+2048,243+2048,318+2048,398+2048,482+2048,572+2048,667+2048,767+2048
frq_908:   .dw  908,1023,60+2048,125+2048,193+2048,265+2048,342+2048,423+2048,509+2048,601+2048,697+2048,800+2048
frq_909:   .dw  909,0+2048,61+1025,125+2048,194+2048,266+2048,343+2048,424+2048,510+2048,601+2048,698+2048,801+2048
frq_916:   .dw  916,4+2048,65+2048,130+2048,198+2048,271+2048,347+2048,429+2048,515+2048,607+2048,704+2048,807+2048
frq_918:   .dw  918,5+2048,66+2048,131+2048,199+2048,272+2048,349+2048,431+2048,517+2048,609+2048,706+2048,809+2048
frq_920:   .dw  920,6+2048,67+2048,132+2048,201+2048,274+2048,351+2048,433+2048,519+2048,611+2048,708+2048,811+2048
frq_921:   .dw  921,7+2048,68+2048,133+2048,202+2048,274+2048,352+2048,433+2048,520+2048,612+2048,709+2048,812+2048
frq_925:   .dw  925,8+2048,70+2048,135+2048,204+2048,277+2048,354+2048,436+2048,523+2048,615+2048,712+2048,815+2048
frq_926:   .dw  926,9+2048,70+2048,135+2048,204+2048,277+2048,355+2048,437+2048,523+2048,615+2048,713+2048,816+2048
frq_927:   .dw  927,10+2048,71+2048,136+2048,205+2048,278+2048,356+2048,438+2048,525+2048,617+2048,715+2048,818+2048
frq_929:   .dw  929,11+2048,72+2048,137+2048,206+2048,280+2048,357+2048,439+2048,526+2048,618+2048,716+2048,820+2048
frq_931:   .dw  931,12+2048,73+2048,138+2048,208+2048,281+2048,358+2048,441+2048,528+2048,620+2048,718+2048,821+2048
frq_935:   .dw  935,13+2048,75+2048,141+2048,210+2048,283+2048,361+2048,443+2048,530+2048,623+2048,721+2048,825+2048
frq_936:   .dw  936,14+2048,76+2048,142+2048,211+2048,284+2048,362+2048,444+2048,532+2048,624+2048,722+2048,826+2048
frq_937:   .dw  937,15+2048,77+2048,142+2048,211+2048,285+2048,363+2048,445+2048,533+2048,625+2048,723+2048,827+2048
frq_939:   .dw  939,16+2048,78+2048,143+2048,213+2048,286+2048,364+2048,447+2048,534+2048,627+2048,725+2048,829+2048
frq_941:   .dw  941,17+2048,79+2048,144+2048,214+2048,287+2048,365+2048,448+2048,535+2048,628+2048,726+2048,830+2048
frq_942:   .dw  942,18+2048,80+2048,145+2048,215+2048,288+2048,367+2048,449+2048,537+2048,630+2048,728+2048,832+2048
frq_944:   .dw  944,19+2048,80+2048,146+2048,216+2048,289+2048,368+2048,450+2048,538+2048,631+2048,729+2048,834+2048
frq_949:   .dw  949,21+2048,83+2048,149+2048,218+2048,292+2048,371+2048,454+2048,542+2048,635+2048,733+2048,838+2048
frq_950:   .dw  950,22+2048,84+2048,150+2048,219+2048,293+2048,372+2048,455+2048,543+2048,636+2048,734+2048,839+2048
frq_951:   .dw  951,22+2048,85+2048,151+2048,220+2048,294+2048,372+2048,456+2048,543+2048,637+2048,736+2048,841+2048
frq_952:   .dw  952,23+2048,85+2048,151+2048,221+2048,295+2048,373+2048,456+2048,544+2048,638+2048,736+2048,841+2048
frq_953:   .dw  953,23+2048,86+2048,152+2048,221+2048,296+2048,374+2048,457+2048,545+2048,639+2048,737+2048,842+2048
frq_964:   .dw  964,29+2048,92+2048,158+2048,228+2048,303+2048,382+2048,466+2048,554+2048,648+2048,747+2048,852+2048
frq_966:   .dw  966,30+2048,93+2048,159+2048,229+2048,304+2048,383+2048,467+2048,555+2048,649+2048,749+2048,854+2048
frq_969:   .dw  969,32+2048,95+2048,161+2048,232+2048,306+2048,385+2048,469+2048,558+2048,652+2048,752+2048,857+2048
frq_974:   .dw  974,34+2048,97+2048,164+2048,235+2048,310+2048,389+2048,473+2048,562+2048,656+2048,756+2048,862+2048
frq_977:   .dw  977,36+2048,99+2048,166+2048,237+2048,312+2048,391+2048,475+2048,564+2048,659+2048,759+2048,865+2048
frq_978:   .dw  978,37+2048,100+2048,167+2048,238+2048,313+2048,392+2048,476+2048,565+2048,660+2048,760+2048,866+2048
frq_1018:  .dw  1018,58+2048,122+2048,190+2048,263+2048,339+2048,420+2048,506+2048,597+2048,693+2048,796+2048,904+2048
