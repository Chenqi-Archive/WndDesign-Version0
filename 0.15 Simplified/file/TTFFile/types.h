#pragma once

typedef unsigned int		uint;
typedef unsigned char		uchar;

typedef char				CHAR;
typedef unsigned char       UCHAR;
typedef short				SHORT;
typedef unsigned short      USHORT;
typedef unsigned long       ULONG;
typedef unsigned long long	ULONGLONG;

typedef UCHAR				UBYTE;
typedef ULONG				Fixed;
typedef ULONGLONG			LONGDATETIME;
typedef SHORT				FWORD;
typedef USHORT				UFWORD;

inline void byte_swap(UBYTE& val) { 
	// Do nothing.
}

inline void byte_swap(USHORT& val) {
	val = (val << 8) | (val >> 8);
}

inline void byte_swap(ULONG& val) {
	val = (val << 24) | ((val << 8) & 0x00ff0000) | ((val >> 8) & 0x0000ff00) | (val >> 24);
}

inline void byte_swap(ULONGLONG& val) {
	ULONG high = val >> 32;
	ULONG low  = (ULONG)val;
	byte_swap(high);
	byte_swap(low);
	val = (((ULONGLONG)low) << 32) | high;
}

inline void byte_swap(SHORT& val) { byte_swap((USHORT&)val); }


template<class _key>
inline void ReadVar(_key& var, const void* data, uint& offset) {
	var = *(_key*)((char*)data + offset);
	byte_swap(var);
	offset += sizeof(_key);
}

// with data(void*) and offset(uint)
#define ReadVar(var) ReadVar(var, data, offset)

#define ReadVarArray(_array, length) for(uint i=0;i<length;++i) {ReadVar(_array[i]);}

#define ReadVarArrayNew(type, _array, length) \
	_array = new type[length]; \
	for(uint i=0;i<length;++i) {ReadVar(_array[i]);}