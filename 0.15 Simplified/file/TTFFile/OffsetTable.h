#pragma once

#include "types.h"

struct OffsetTable {
	Fixed	SFNT_Ver;		//sfnt version 0x00010000 for version 1.0. 
	USHORT  numTables;		//Number of tables.  
	USHORT  searchRange;	//(Maximum power of 2 <= numTables) x 16. 
	USHORT  entrySelector;	// Log2(maximum power of 2 <= numTables). 
	USHORT  rangeShift;		// NumTables x 16-searchRange.

	void read(const void* const data, uint offset) {
		ReadVar(SFNT_Ver);
		ReadVar(numTables);
		ReadVar(searchRange);
		ReadVar(entrySelector);
		ReadVar(rangeShift);
	}
};


struct EntryTable {
	union {
		char asChar[4];		// 4 -byte identifier. 
		ULONG asLong;
	} Tag;
	ULONG checkSum;			// CheckSum for this table. 
	ULONG offset;			// Offset from beginning of TrueType font file. 
	ULONG length;			// Length of this table. 

	void read(const void* const data, uint offset) {
		Tag.asLong = *(ULONG*)((char*)data + offset); offset += sizeof(ULONG);
		ReadVar(checkSum);
		ReadVar(this->offset);
		ReadVar(length);
	}
};

