#pragma once

#include "types.h"

struct hheaTable {
	Fixed version;		// Table version number 0x00010000 for version 1.0. 
	FWORD Ascender;	// Typographic ascent. (Distance from baseline of highest ascender)  
	FWORD Descender;	// Typographic descent. (Distance from baseline of lowest descender)  
	FWORD LineGap;	// Typographic line gap. 
		//Negative LineGap values are treated as zero 
		//in Windows 3.1, System 6, and 
		//System 7. 
	UFWORD advanceWidthMax;		// Maximum advance width value in 'hmtx' table. 
	FWORD minLeftSideBearing;	// Minimum left sidebearing value in 'hmtx' table. 
	FWORD minRightSideBearing;	// Minimum right sidebearing value; calculated as Min(aw - lsb - (xMax - xMin)). 
	FWORD xMaxExtent;	// Max(lsb + (xMax - xMin)). 
	SHORT caretSlopeRise;	// Used to calculate the slope of the cursor (rise/run); 1 for vertical. 
	SHORT caretSlopeRun;	// 0 for vertical. 
	SHORT caretOffset;		// The amount by which a slanted highlight on a glyph needs to be shifted to produce the best appearance. Set to 0 for non-slanted fonts 
	SHORT reserved0;	// set to 0 
	SHORT reserved1;	// set to 0 
	SHORT reserved2;	// set to 0 
	SHORT reserved3;	// set to 0 
	SHORT metricDataFormat;	// 0 for current format. 
	USHORT numberOfHMetrics;	// Number of hMetric entries in 'hmtx' table 

	void read(const void* const data, uint offset) {
		ReadVar(version);
		ReadVar(Ascender);
		ReadVar(Descender);
		ReadVar(LineGap);
		ReadVar(advanceWidthMax);
		ReadVar(minLeftSideBearing);
		ReadVar(minRightSideBearing);
		ReadVar(xMaxExtent);
		ReadVar(caretSlopeRise);
		ReadVar(caretSlopeRun);
		ReadVar(caretOffset);
		ReadVar(reserved0);
		ReadVar(reserved1);
		ReadVar(reserved2);
		ReadVar(reserved3);
		ReadVar(metricDataFormat);
		ReadVar(numberOfHMetrics);
	}			
};