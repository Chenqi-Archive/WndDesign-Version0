#pragma once

#include "types.h"

typedef struct tOS_2 {
	// http://www.microsoft.com/typography/OTSPEC/os2.htm
	USHORT version;	// 0x0003 
	SHORT xAvgCharWidth;
	USHORT usWeightClass;
	USHORT usWidthClass;
	USHORT fsType;
	SHORT ySubscriptXSize;
	SHORT ySubscriptYSize;
	SHORT ySubscriptXOffset;
	SHORT ySubscriptYOffset;
	SHORT ySuperscriptXSize;
	SHORT ySuperscriptYSize;
	SHORT ySuperscriptXOffset;
	SHORT ySuperscriptYOffset;
	SHORT yStrikeoutSize;
	SHORT yStrikeoutPosition;
	SHORT sFamilyClass;
	struct tpanose {
		UBYTE bFamilyType;
		UBYTE bSerifStyle;
		UBYTE bWeight;
		UBYTE bProportion;
		UBYTE bContrast;
		UBYTE bStrokeVariation;
		UBYTE bArmStyle;
		UBYTE bLetterform;
		UBYTE bMidline;
		UBYTE bXHeight;
	} panose;
	ULONG ulUnicodeRange1;	// Bits 0-31 
	ULONG ulUnicodeRange2;	// Bits 32-63 
	ULONG ulUnicodeRange3;	// Bits 64-95 
	ULONG ulUnicodeRange4;	// Bits 96-127 
	CHAR achVendID[4];
	USHORT fsSelection;
	USHORT usFirstCharIndex;
	USHORT usLastCharIndex;
	SHORT sTypoAscender;
	SHORT sTypoDescender;
	SHORT sTypoLineGap;
	USHORT usWinAscent;
	USHORT usWinDescent;
	ULONG ulCodePageRange1;	// Bits 0-31 
	ULONG ulCodePageRange2;	// Bits 32-63 
	SHORT sxHeight;
	SHORT sCapHeight;
	USHORT usDefaultChar;
	USHORT usBreakChar;
	USHORT usMaxContext;
};