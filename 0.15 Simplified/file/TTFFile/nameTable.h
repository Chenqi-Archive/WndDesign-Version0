#pragma once

#include "types.h"

struct NameRecord {
	USHORT platformID;	// Platform ID. 
	USHORT encodingID;	// Platform-specific encoding ID. 
	USHORT languageID;	// Language ID. 
	USHORT nameID;		// Name ID. 
	USHORT length;		// String length (in bytes). 
	USHORT offset;		// String offset from start of storage area (in bytes). 
	
	char* name;

	void read(const void* const data, uint offset) {
		ReadVar(platformID);
		ReadVar(encodingID);
		ReadVar(languageID);
		ReadVar(nameID);
		ReadVar(length);
		ReadVar(this->offset);

		name = new char[length]; // may check the encoding.
		for (uint i = 0; i < length; ++i) {
			name[i] = ((char*)data + this->offset)[i];
		}
	}
	~NameRecord() { delete[] name; }
};

struct nameTable {
	USHORT		format;			// Format selector (=0). 
	USHORT		count;			// Number of name records. 
	USHORT		stringOffset;	// Offset to start of string storage (from start of table). 
	NameRecord* records;

	void read(const void* const data, uint offset) {
		ReadVar(format);
		ReadVar(count);
		ReadVar(stringOffset);

		records = new NameRecord[count];
		for (uint i = 0; i < count; ++i) {
			records[i].read(data, offset);
			offset += sizeof(NameRecord) - sizeof(char*);
		}
	}
	~nameTable() { delete[] records; }
};

