#include "File.h"
#include <Windows.h>

WndDesign::File::File() :hFile(INVALID_HANDLE_VALUE), hFileMap(NULL), pFile(nullptr), size() {

}

bool WndDesign::File::Open(const wchar_t* fileName) {
	if (hFile != INVALID_HANDLE_VALUE || hFileMap != NULL || pFile != nullptr) {
		return false;
	}

	hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
					   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	}

	if (!GetFileSizeEx(hFile, (PLARGE_INTEGER)&size)) {
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
		return false;
	}

	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY,
								 size.HighPart, size.LowPart, NULL);
	if (hFileMap == NULL) {
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
		return false;
	}

	pFile = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
	if (pFile == nullptr) {
		CloseHandle(hFileMap); hFileMap = NULL;
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
		return false;
	}

	return true;
}

bool WndDesign::File::Edit(const wchar_t* fileName) {
	if (hFile != INVALID_HANDLE_VALUE || hFileMap != NULL || pFile != nullptr) {
		return false;
	}

	hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
					   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	}

	if (!GetFileSizeEx(hFile, (PLARGE_INTEGER)&size)) {
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
		return false;
	}

	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE,
								 size.HighPart, size.LowPart, NULL);
	if (hFileMap == NULL) {
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
		return false;
	}

	pFile = MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (pFile == nullptr) {
		CloseHandle(hFileMap); hFileMap = NULL;
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
		return false;
	}

	return true;
}

bool WndDesign::File::Create(const wchar_t* fileName, uint size, bool overwrite) {
	if (hFile != INVALID_HANDLE_VALUE || hFileMap != NULL || pFile != nullptr) {
		return false;
	}

	hFile = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
					   (overwrite ? CREATE_ALWAYS : CREATE_NEW), FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	}

	this->size.LowPart = size;

	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, size, NULL);
	if (hFileMap == NULL) {
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
		return false;
	}

	pFile = MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (pFile == nullptr) {
		CloseHandle(hFileMap); hFileMap = NULL;
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
		return false;
	}

	return true;
}

void WndDesign::File::Close() {
	if (pFile != nullptr) {
		UnmapViewOfFile(pFile); pFile = nullptr;
	}
	if (hFileMap != NULL) {
		CloseHandle(hFileMap); hFileMap = NULL;
	}
	if (hFile != INVALID_HANDLE_VALUE) {
		CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE;
	}
	size.QuadPart = 0LL;
}

WndDesign::File::~File() {
	Close();
}


bool WndDesign::OpenFileDialog(wchar_t* fileName, uint length) {
	OPENFILENAME file = {};
	file.lStructSize = sizeof(file);
	//file.hwndOwner = NULL;  // Modeless dialog.
	//file.hInstance = NULL;
	//file.lpstrTitle = NULL;

	file.lpstrFile = fileName;
	file.nMaxFile = length;

	file.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	//file.lpstrFilter = NULL;
	//file.nFilterIndex = 0;

	//file.lpstrInitialDir = NULL;
	//file.lpstrDefExt = NULL;
	//file.lpTemplateName = NULL;
	//file.lpfnHook = NULL;
	//file.lCustData = NULL;

	if (!GetOpenFileName(&file)) { 
		return false; 
	} else {
		return true;
	}
}

bool WndDesign::OpenFolderDialog(wchar_t* folderName, uint length) {
	return false;
}
