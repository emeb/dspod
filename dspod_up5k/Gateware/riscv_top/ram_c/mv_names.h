/*
 * mv_names.h
 * names of Midiverb Programs
 */
 
const char *mv_prog_names[] =
{
	// MIDIVerb I algorithms
	"0.2 S Br",		// 1
	"0.2 S Wm",		// 2
	"0.2 M Br",		// 3
	"0.3 S Br",		// 4
	"0.3 S Ww",		// 5
	"0.4 M Br",		// 6
	"0.4 M Wm",		// 7
	"0.6 S Br",		// 8
	"0.6 M Wm",		// 9
	"0.6 M Dk",		// 10
	"0.8 S Br",		// 11
	"0.8 L Wm",		// 12
	"1.0 S Ww",		// 13
	"1.0 M Ww",		// 14
	"1.0 L Br",		// 15
	"1.2 M Wm",		// 16
	"1.2 S Wm",		// 17
	"1.2 S Br",		// 18
	"1.4 L Wm",		// 19
	"1.4 L Dk",		// 20
	"1.4 M Wm",		// 21
	"1.6 S Dk",		// 22
	"1.6 L Br",		// 23
	"1.6 M Br",		// 24
	"1.8 L Dk",		// 25
	"1.8 L Br",		// 26
	"1.8 M Wm",		// 27
	"2.0 L Br",		// 28
	"2.0 M Wm",		// 29
	"2.0 L Wm",		// 30
	"2.5 M Wm",		// 31
	"2.5 L Br",		// 32
	"2.5 M Dk",		// 33
	"2.8 S Br",		// 34
	"2.8 M Br",		// 35
	"3.0 L Br",		// 36
	"3.0 L Wm",		// 37
	"3.0 M Dk",		// 38
	"3.5 L Br",		// 39
	"3.5 M Wm",		// 40
	"4.0 M Dk",		// 41
	"4.0 M Br",		// 42
	" 5  L Wm",		// 43
	" 8  L Br",		// 44
	" 8  L Wm",		// 45
	"10  L Br",		// 46
	"10  L Wm",		// 47
	"16  L Dk",		// 48
	"18  X Br",		// 49
	"20  X Dk",		// 50
	"Gate 100",		// 51
	"Gate 150",		// 52
	"Gate 200",		// 53
	"Gate 250",		// 54
	"Gate 300",		// 55
	"Gate 350",		// 56
	"Gate 400",		// 57
	"Gate 500",		// 58
	"Gate 600",		// 59
	"Revs 300",		// 60
	"Revs 400",		// 61
	"Revs 500",		// 62
	"Revs 600",		// 63
	"Defeat  ",		// 64
	
	// MIDIFex Algorithms
	"Echo LFA",		// 1  ECHO LONG FLAT AMBI
	"Echo LFT",		// 2  ECHO LONG FLAT THICK
	"Echo LH ",		// 3  ECHO LONG HPF 
	"Echo LHW",		// 4  ECHO LONG HPF WIDE
	"Echo LBA",		// 5  ECHO LONG BPF AMBI
	"Echo LLW",		// 6  ECHO LONG LPF WIDE
	"Echo MFA",		// 7  ECHO MED FLAT AMBI
	"Echo MFW",		// 8  ECHO MED FLAT WIDE
	"Echo MHA",		// 9  ECHO MED HPF AMBI
	"Echo MBA",		// 10 ECHO MED BPF AMBI
	"Echo MLA",		// 11 ECHO MED LPF AMBI
	"Echo MLW",		// 12 ECHO MED LPF WIDE
	"Echo MFT",		// 13 ECHO MED FLAT THICK
	"Echo SF ",		// 14 ECHO SHORT FLAT
	"Echo SLA",		// 15 ECHO SHORT LPF AMBI
	"Echo SHA",		// 16 ECHO SHORT HPF AMBI
	"Echo SBA",		// 17 ECHO SHORT BPF AMBI
	"Echo SF ",		// 18 ECHO SHORT FLAT
	"Echo SFW",		// 19 ECHO SHORT FLAT WIDE
	"Echo XF ",		// 20 ECHO XSHORT FLAT
	"Echo XB ",		// 21 ECHO XSHORT BPF
	"2Tap MFA",		// 22 2 TAP MED FLAT AMBI
	"2Tap MHA",		// 23 2 TAP MED HPF AMBI
	"2Tap MBA",		// 24 2 TAP MED BPF AMBI
	"2Tap MFT",		// 25 2 TAP MED FLAT THICK
	"2Tap SFW",		// 26 2 TAP SHORT FLAT WIDE
	"2Tap SHP",		// 27 2 TAP SHORT HPF PAN
	"2Tap SBA",		// 28 2 TAP SHORT BPF AMBI
	"2Tap SLA",		// 29 2 TAP SHORT LPF AMBI
	"2Tap XFW",		// 30 2 TAP XSHORT FLAT WIDE
	"3Tap MFP",		// 31 3 TAP MED FLAT PAN
	"3Tap SFP",		// 32 3 TAP SHORT FLAT PAN
	"3Tap SLA",		// 33 3 TAP SHORT LPF AMBI
	"3Tap SBA",		// 34 3 TAP SHORT BPF AMBI
	"3Tap SHA",		// 35 3 TAP SHORT HPF AMBI
	"3Tap XFA",		// 36 3 TAP XSHORT FLAT AMBI
	"Regn MF ",		// 37 REGEN MED FLAT
	"Regn MHA",		// 38 REGEN MED HPF AMBI
	"Regn MBA",		// 39 REGEN MED BPF AMBI
	"Regn MLA",		// 40 REGEN MED LPF AMBI
	"Regn MSF",		// 41 REGEN SHORT FLAT
	"Regn MXF",		// 42 REGEN XSHORT FLAT
	"Slap 1  ",		// 43 SLAP1
	"Slap 2  ",		// 44 SLAP2
	"Slap 3  ",		// 45 SLAP3
	"Slap 4  ",		// 46 SLAP4
	"Slap 5  ",		// 47 SLAP5
	"Revb SG ",		// 48 REVERB SHORT GATE
	"Revb MW ",		// 49 REVERB MEDIUM WARM
	"Revb MB ",		// 50 REVERB MEDIUM BLOOM
	"Revb MP ",		// 51 REVERB MEDIUM PAN
	"Revb LH ",		// 52 REVERB LONG HPF
	"Revb Rv ",		// 53 REVERB REVERSE
	"Revb RvR",		// 54 REVERB REVERSE REGEN
	"Mlttp P ",		// 55 MULTITAP PAN
	"Mlttp R ",		// 56 MULTITAP REVERB
	"Mlttp RP",		// 57 MULTITAP REVERSE PAN
	"Thcknr  ",		// 58 THICKENER
	"Thcknr D",		// 59 THICKENER DENSE
	"StrGen A",		// 60 STEREOGEN AMBIENT
	"StrGen T",		// 61 STEREOGEN THICK
	"StrGen W",		// 62 STEREOGEN WIDE
	"StrGen X",		// 63 STEREOGEN XWIDE
	"Defeat  ",		// 64
	
	// Special custom
	"Dly 1 1 ",		// 1 1-8 = single repeat delay
	"Dly 1 2 ",		// 2
	"Dly 1 3 ",		// 3
	"Dly 1 4 ",		// 4
	"Dly 1 5 ",		// 5
	"Dly 1 6 ",		// 6
	"Dly 1 7 ",		// 7
	"Dly 1 8 ",		// 8
	"Dly 2 1 ",		// 9 9-13 = two repeat delay
	"Dly 2 2 ",		// 10
	"Dly 2 3 ",		// 11
	"Dly 2 4 ",		// 12
	"Dly 2 5 ",		// 13
	"Dly 3 1 ",		// 14 14-15 = three repeat delay
	"Dly 3 2 ",		// 15
	"MltRep 1",		// 16 16-19 = multiple repeat delay
	"MltRep 2",		// 17
	"MltRep 3",		// 18
	"MltRep 4",		// 19
	"MltRnd 1",		// 20 20-21 = multitap random delays
	"MltRnd 2",		// 21
	"PP/Str 1",		// 22 22-25 = ping-pong/stereo panning effects
	"PP/Str 2",		// 23
	"PP/Str 3",		// 24
	"PP/Str 4",		// 25
	"StrGen 1",		// 26 26-31 = stereo simulator effects
	"StrGen 2",		// 27
	"StrGen 3",		// 28
	"StrGen 4",		// 29
	"StrGen 5",		// 30
	"StrGen 6",		// 31
	"Revb G 1",		// 32 32-37 = various gated reverbs
	"Revb G 2",		// 33
	"Revb G 3",		// 34
	"Revb G 4",		// 35
	"Revb G 5",		// 36
	"Revb G 6",		// 37
	"Revb 1  ",		// 38 38-58 = various reverbs
	"Revb 2  ",		// 39
	"Revb 3  ",		// 40
	"Revb 4  ",		// 41
	"Revb 5  ",		// 42
	"Revb 6  ",		// 43
	"Revb 7  ",		// 44
	"Revb 8  ",		// 45
	"Revb 9  ",		// 46
	"Revb 10 ",		// 47
	"Revb 11 ",		// 48
	"Revb 12 ",		// 49
	"Revb 13 ",		// 50
	"Revb 14 ",		// 51
	"Revb 15 ",		// 52
	"Revb 16 ",		// 53
	"Revb 17 ",		// 54
	"Revb 18 ",		// 55
	"Revb 19 ",		// 56
	"Revb 20 ",		// 57
	"Revb 21 ",		// 58
	"Revb R 1",		// 59 59-63 = backwards reverbs
	"Revb R 2",		// 60
	"Revb R 3",		// 61
	"Revb R 4",		// 62
	"Revb R 5",		// 63
	"Defeat  ",		// 64
	
	// MV II
	"0.1 S Br",		// 1
	"0.2 S Br",		// 2
	"0.3 S Br",		// 3
	"1.1 M Wm",		// 4
	"0.6 M Br",		// 5
	"1.2 L Br",		// 6
	"1.0 L Dk",		// 7
	"0.6 M Dk",		// 8
	"0.5 M Dk",		// 9
	"0.6 M Br",		// 10
	"0.8 M Br",		// 11
	"1.0 L Wm",		// 12
	"1.1 L Wm",		// 13
	"1.0 M Dk",		// 14
	"1.1 M Br",		// 15
	"1.2 M Br",		// 16
	"1.6 L Br",		// 17
	"1.7 L Dk",		// 18
	"1.7 M Br",		// 19
	"1.9 M Br",		// 20
	"2.2 L Wm",		// 21
	"1.8 L Wm",		// 22
	"1.5 L Br",		// 23
	"2.2 L Dk",		// 24
	"2.3 L Wm",		// 25
	"2.4 L Br",		// 26
	"2.5 L Br",		// 27
	"5.0 X Wm",		// 28
	"15  X Wm",		// 29
	"SwGt 100",		// 30
	"SwGt 200",		// 31
	"SwGt 250",		// 32
	"SwGt 400",		// 33
	"SwGt 450",		// 34
	"FsGt 150",		// 35
	"FsGt  75",		// 36
	"FsGt 200",		// 37
	"FsGt 100",		// 38
	"FsGt 175",		// 39
	"Revs 400",		// 40
	"Revs 450",		// 41
	"Revs 250",		// 42
	"Revs 200",		// 43
	"Revs 150",		// 44
	"Blm 1 8s",		// 45
	"Revs 500",		// 46
	"RvRgn 2s",		// 47
	"RvRgn 8s",		// 48
	"Blm 2 7s",		// 49
	"TrgFlg 1",		// 50
	"FlgPan 1",		// 51
	"FlgPan 2",		// 52
	"TrgFlg 2",		// 53
	"FlgPan 3",		// 54
	"FlgPan 4",		// 55
	"FlgPan 5",		// 56
	"TrgFlg 3",		// 57
	"FlgPan 6",		// 58
	"TrgFlg 7",		// 59
	"L Chrs 1",		// 60
	"L Chrs 2",		// 61
	"M Chrs 1",		// 62
	"M Chrs 2",		// 63
	"M Chrs 3",		// 64
	"D Chrs 1",		// 65
	"D Chrs 2",		// 66
	"D Chrs 3",		// 67
	"F Chrs 1",		// 68
	"F Chrs 2",		// 69
	"Dly   35",		// 70
	"Dly   55",		// 71
	"Dly   75",		// 72
	"Dly  115",		// 73
	"Dly  140",		// 74
	"Dly  155",		// 75
	"Dly  160",		// 76
	"Dly  170",		// 77
	"Dly  175",		// 78
	"Dly  180",		// 79
	"Dly  185",		// 80
	"Dly  195",		// 81
	"Dly  205",		// 82
	"Dly  210",		// 83
	"Dly  220",		// 84
	"Dly  240",		// 85
	"Dly  250",		// 86
	"Dly  275",		// 87
	"Dly  375",		// 88
	"Dly  450",		// 89
	"2tap Amb",		// 90
	"3tap Pan",		// 91
	"Multitap",		// 92
	"Mtp RPan",		// 93
	"ThkFrzFl",		// 94
	"StGen   ",		// 95
	"StGen Wd",		// 96
	"RgDly 2s",		// 97
	"RgDly 3s",		// 98
	"RgDly 4s",		// 99
	
};


