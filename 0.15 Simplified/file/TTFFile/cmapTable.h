#pragma once

#include "types.h"

#pragma pack(push, 2)

struct campFormatCommon {
	USHORT format;				// Format number is set to 0, 2, 4, 6...  
	USHORT length;				// This is the length in bytes of the subtable. 
	USHORT language;			// Please see "Note on the language field in 'cmap' subtables" in this document. 

	void read(const void* const data, uint offset) {
		ReadVar(format);
		ReadVar(length);
		ReadVar(language);
	}
};

struct cmapFormat0 {
	UBYTE glyphIdArray[256];	// An array that maps character codes to glyph index values. 
	void read(const void* const data, uint offset, uint length) {
		for (uint i = 0; i < 256; ++i) {
			ReadVar(glyphIdArray[i]);
		}
	}
};
struct cmapFormat2 {

	void read(const void* const data, uint offset, uint length) {
		//
	}
};
struct cmapFormat4 {
	USHORT	segCountX2;		// 2 x segCount. 
	USHORT	searchRange;	// 2 x (2**floor(log2(segCount))) 
	USHORT	entrySelector;	// log2(searchRange/2) 
	USHORT	rangeShift;		// 2 x segCount - searchRange 
	USHORT* endCode;		// End characterCode for each segment, last=0xFFFF. 
	USHORT	reservedPad;	// Set to 0. 
	USHORT* startCode;		// Start character code for each segment. 
	SHORT*	idDelta;		// Delta for all character codes in segment. 
	USHORT* idRangeOffset;	// Offsets into glyphIdArray or 0 
	//USHORT* glyphIdArray;	// Glyph index array (arbitrary length) 

	void read(const void* const data, uint offset, uint length) {
		uint offset0 = offset;

		ReadVar(segCountX2);
		ReadVar(searchRange);
		ReadVar(entrySelector);
		ReadVar(rangeShift);

		USHORT segCount = segCountX2 / 2;
		ReadVarArrayNew(USHORT, endCode, segCount);
		ReadVar(reservedPad);
		ReadVarArrayNew(USHORT, startCode, segCount);
		ReadVarArrayNew(SHORT, idDelta, segCount);
		ReadVarArrayNew(USHORT, idRangeOffset, segCount + (length - (offset - offset0)) / 2);
		
		//ReadVarArrayNew(USHORT, glyphIdArray, (length - (offset - offset0)) / 2);
	}

	USHORT find(wchar_t ch) {
		if (endCode == nullptr) { return 0; }

		USHORT segCount = segCountX2 / 2;

		// Search for the first endCode that >= the character code.
		uint i = 0;
		for (; i < segCount; ++i) {
			if (endCode[i] >= ch) { 
				break; 
			}
		}  // May use binary search here.

		if (startCode[i] > ch || ch == 0xFFFF) { return 0; } // missing

		if (idRangeOffset[i] == 0) {
			return ch + idDelta[i];
		} else {
			USHORT glyphId = *(&idRangeOffset[i] + idRangeOffset[i] / 2 + (ch - startCode[i])); // 
			if (glyphId == 0) { return 0; } // missing
			return glyphId + idDelta[i];
		}
	}

	~cmapFormat4() {
		delete[]endCode;
		delete[]startCode;
		delete[]idDelta;
		delete[]idRangeOffset;
	}
};
struct cmapFormat6 {
	USHORT firstCode;		// First character code of subrange. 
	USHORT entryCount;		// Number of character codes in subrange. 
	USHORT* glyphIdArray;	// Array of glyph index values for character codes in the range. 
	void read(const void* const data, uint offset, uint length) {
		ReadVar(firstCode);
		ReadVar(entryCount);
		ReadVarArrayNew(USHORT, glyphIdArray, entryCount);
	}
	~cmapFormat6() { delete[] glyphIdArray; }
};
struct cmapFormat8 {

	void read(const void* const data, uint offset, uint length) {

	}
}; 
struct cmapFormat10 {

	void read(const void* const data, uint offset, uint length) {

	}
}; 
struct cmapFormat12 {

	void read(const void* const data, uint offset, uint length) {

	}
};


struct EncodingRecord {
	USHORT	platformID;	// Platform ID. 
	USHORT	encodingID;	// Platform-specific encoding ID. 
	ULONG	offset;		// Byte offset from beginning of table to the subtable for this encoding. 
	
	campFormatCommon formatInfo;
	union {
		cmapFormat0* format0;
		cmapFormat2* format2;
		cmapFormat4* format4;
		cmapFormat6* format6;
		cmapFormat8* format8;
		cmapFormat10* format10;
		cmapFormat12* format12;
	};

	void read(const void* const data, uint offset, uint campTbOffset) {
		ReadVar(platformID);
		ReadVar(encodingID);
		ReadVar(this->offset);

		offset = campTbOffset + this->offset;
		formatInfo.read(data, offset);
		offset += sizeof(campFormatCommon);

		switch (formatInfo.format) {

		#define ReadcampFormat(num) \
			case num: \
				format##num = new cmapFormat##num(); \
				format##num->read(data, offset, formatInfo.length); \
				break;
			
			ReadcampFormat(0);
			ReadcampFormat(2);
			ReadcampFormat(4);
			ReadcampFormat(6);
			ReadcampFormat(8);
			ReadcampFormat(10);
			ReadcampFormat(12);
		}
	}

	~EncodingRecord() {
		switch (formatInfo.format) {
			case 0: delete format0; format0 = nullptr; break;
			case 2: delete format2; format2 = nullptr; break;
			case 4: delete format4; format4 = nullptr; break;
			case 6: delete format6; format6 = nullptr; break;
			case 8: delete format8; format8 = nullptr; break;
			case 10: delete format10; format10 = nullptr; break;
			case 12: delete format12; format12 = nullptr; break;
		}
	}
};

struct cmapTable {
	USHORT version;		// Table version number (0).  
	USHORT numTables;	// Number of encoding tables that follow. 
	EncodingRecord* records;

	void read(const void* const data, uint offset) {
		uint campTbOffset = offset;

		ReadVar(version);
		ReadVar(numTables);

		records = new EncodingRecord[numTables]();
		for (uint i = 0; i < numTables; ++i) {
			records[i].read(data, offset, campTbOffset);
			offset += sizeof(EncodingRecord) - sizeof(void*) - sizeof(campFormatCommon);
		}
	}

	USHORT find(wchar_t ch) const {
		if (records == nullptr) { return 0; }

		for (uint i = 0; i < numTables; ++i) {
			EncodingRecord& record = records[i];

			// Windows - Unicode BMP (Format 4)
			if (record.platformID == 3 && record.encodingID == 1 && record.formatInfo.format == 4) {
				return record.format4->find(ch);
			}
		}

		return 0;
	}

	~cmapTable() { delete[]records; }
};

#pragma pack(pop)