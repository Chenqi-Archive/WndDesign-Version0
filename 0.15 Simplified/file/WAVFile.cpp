#include "WAVFile.h"
#include <cstring>

bool WndDesign::WAVFile::Open(const wchar_t* fileName) {
	if (!File::Open(fileName)) { return false; }
	do {
		if (Size().HighPart != 0) { break; }

		uint size = Size().LowPart;
		uint offset = 0;
		const char* data = (char*)PFile();

		if (size - offset < sizeof(RiffChunk)) { break; }

		RiffChunk* riffHeader = (RiffChunk*)(data + offset);
		if (riffHeader->chunkID.val != 0x46464952) { break; }  // "RIFF"
		if (riffHeader->size != size) { break; }
		offset += sizeof(RiffChunk);

		RiffChunk::ChunkID* riffType = (RiffChunk::ChunkID*)(data + offset);
		if (riffType->val != 0x45564157) { break; }  // "WAVE"
		offset += sizeof(RiffChunk::ChunkID);

		RiffChunk* formatChunk = (RiffChunk*)(data + offset);
		if (formatChunk->chunkID.val != 0x20746D66) { break; }  // "fmt "
		offset += sizeof(RiffChunk);

		this->format = (WAVFORMAT*)(data + offset);
		if (format->wFormatTag != 1) { break; } // PCM format
		if (format->wBlockAlign != format->wChannels * format->wBitsPerSample / 8) { break; }
		if (format->dwAvgBytesPerSec != format->wBlockAlign * format->dwSamplesPerSec) { break; }
		offset += formatChunk->size;

		RiffChunk* dataChunk = (RiffChunk*)(data + offset);
		if (dataChunk->chunkID.val != 0x61746164) { break; }  // "data"
		offset += sizeof(RiffChunk);

		this->dataSize = dataChunk->size;
		this->data = (void*)(data + offset);

		return true;
	} while (0);
//Error:
	Close();
	return false;
}

bool WndDesign::WAVFile::Create(const wchar_t* fileName, short nChannels, long sampleRate, short bitsPerSample, long dataSize, const void* pcmData) {
	uint size = sizeof(RiffChunk) * 3 + sizeof(RiffChunk::ChunkID) + sizeof(WAVFORMAT) + dataSize;
	if (!File::Create(fileName, size)) { return false; }

	uint offset = 0;
	const char* const data = (char*)PFile();

	RiffChunk* riffHeader = (RiffChunk*)(data + offset);
	riffHeader->chunkID.val = 0x46464952;  // "RIFF"
	riffHeader->size = size - 8;
	offset += sizeof(RiffChunk);

	RiffChunk::ChunkID* riffType = (RiffChunk::ChunkID*)(data + offset);
	riffType->val = 0x45564157;  // "WAVE"
	offset += sizeof(RiffChunk::ChunkID);

	RiffChunk* formatChunk = (RiffChunk*)(data + offset);
	formatChunk->chunkID.val = 0x20746D66;  // "fmt "
	formatChunk->size = 18;
	offset += sizeof(RiffChunk);

	this->format = (WAVFORMAT*)(data + offset);
	this->format->wFormatTag = 1;     // PCM format
	this->format->wChannels = nChannels;
	this->format->dwAvgBytesPerSec = sampleRate;
	this->format->wBitsPerSample = bitsPerSample;
	this->format->unknown = 0;
	this->format->wBlockAlign = this->format->wChannels * this->format->wBitsPerSample / 8;
	this->format->dwAvgBytesPerSec = this->format->wBlockAlign * this->format->dwSamplesPerSec;
	offset += formatChunk->size;

	RiffChunk* dataChunk = (RiffChunk*)(data + offset);
	dataChunk->chunkID.val = 0x61746164;   // "data"
	dataChunk->size = dataSize;
	offset += sizeof(RiffChunk);

	this->dataSize = dataChunk->size;
	this->data = (void*)(data + offset);
	memcpy(this->data, pcmData, dataSize);

	return true;
}

void WndDesign::WAVFile::Close() {
	format = nullptr;
	dataSize = 0;
	data = nullptr;
	File::Close();
}