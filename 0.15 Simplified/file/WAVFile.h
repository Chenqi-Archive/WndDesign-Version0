#pragma once

#include "../sys/File.h"

namespace WndDesign {


#pragma pack(push,2)


typedef struct {
    typedef union {
        uint val;
        char tag[4];
    }ChunkID;

    ChunkID     chunkID;
    long        size;
} RiffChunk;

typedef struct {
    short          wFormatTag;
    unsigned short wChannels;
    unsigned long  dwSamplesPerSec;
    unsigned long  dwAvgBytesPerSec;
    unsigned short wBlockAlign;
    unsigned short wBitsPerSample;
    unsigned short unknown;
} WAVFORMAT;

#pragma pack(pop)

class WAVFile : public File {
private:
    WAVFORMAT* format;
    long dataSize;
    void* data;

public:
    WAVFile():format(nullptr),dataSize(0), data(nullptr) {}
    ~WAVFile() { Close(); }

public:
    bool Open(const wchar_t* fileName);
    bool Create(const wchar_t* fileName,
                short nChannels,
                long sampleRate,
                short bitsPerSample,
                long dataSize,
                const void* pcmData);
    void Close();

public:
    inline unsigned short NumChannels() const {
        if (format == nullptr) { return 0; }
        return format->wChannels;
    }
    inline unsigned long SampleRate() const {
        if (format == nullptr) { return 0; }
        return format->dwSamplesPerSec;
    }
    inline unsigned short BitsPerSample() const { 
        if (format == nullptr) { return 0; } 
        return format->wBitsPerSample;
    }
    inline long DataSize() const {
        if (data == nullptr) { return 0; }
        return dataSize;
    } 
    inline float Length() const { 
        if (format == nullptr) { return 0; } 
        return dataSize / (float)format->dwAvgBytesPerSec;
    }
    inline const void* Data() const { return data; }
};

} // namespace WndDesign