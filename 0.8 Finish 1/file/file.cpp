#include <Windows.h>
#include "file.h"

File::File():hFile(INVALID_HANDLE_VALUE), hFileMap(NULL), pvFile(nullptr){
}

bool File::OpenFile(const wchar_t* fileName){
	if (hFile != INVALID_HANDLE_VALUE || hFileMap != NULL || pvFile != nullptr) {
		CloseFile();
	}

	hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	}

	if (!GetFileSizeEx(hFile, &size)) {
		CloseHandle(hFile);
		return false;
	}

	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE,
		size.HighPart, size.LowPart, NULL);
	if (hFileMap == NULL) {
		CloseHandle(hFile);
		return false;
	}

	pvFile = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (pvFile == NULL) {
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		return false;
	}

	return true;
}

void File::CloseFile() {
	if (pvFile != nullptr) {
		UnmapViewOfFile(pvFile); pvFile = nullptr;
	}
	if (hFileMap != NULL) {
		CloseHandle(hFileMap); hFileMap = NULL;
	}
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
	}
}

File::~File(){
	CloseFile();
}



bool BMPfile::OpenFile(const wchar_t* fileName)
{
	if (!File::OpenFile(fileName)) { goto error; }

	// Suppose the file size is no larger than 4GB.
	if (size.LowPart < sizeof(fileHeader) + sizeof(infoHeader)) { goto error; }

	fileHeader = (BITMAPFILEHEADER*)pvFile;

	if (fileHeader->bfType != 0x4D42) { goto error; }// "BM"
	if (fileHeader->bfSize != size.LowPart) { goto error; }

	data = fileHeader + fileHeader->bfOffBits;




	return true;

error:
	File::CloseFile();
	return false;
}