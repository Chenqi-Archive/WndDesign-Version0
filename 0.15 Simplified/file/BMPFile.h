#pragma once

#include "../sys/File.h"
#include "../basic/type.h"

namespace WndDesign {

#pragma pack(push, 2)

struct BITMAPFILEHEADER {
	union {
		CHAR asChar[2];
		WORD asWord;
	}bfType;
	DWORD bfSize;
	WORD bfResevered1;
	WORD bfResevered2;
	DWORD bfOffBits;
};

struct BITMAPINFOHEADER {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
};

struct RGBTRIPLE{
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
};

struct RGBQUAD {
	BYTE   rgbBlue;
	BYTE   rgbGreen;
	BYTE   rgbRed;
	BYTE   rgbReserved;
};

#pragma pack(pop)

class BMPFile :public File {
	BITMAPFILEHEADER* fileHeader;
	BITMAPINFOHEADER* infoHeader;
	RGBQUAD* palette;
	void* bits;

public:
	BMPFile():fileHeader(nullptr), infoHeader(nullptr), palette(nullptr), bits(nullptr){}
	~BMPFile() { Close(); }

public:
	bool Open(const wchar_t* fileName);
	bool Create(const wchar_t* fileName, const Bitmap& bitmap, bool overwrite = false);
	bool Read(Bitmap& bitmap) const;
	void Close();

};

} // namespace WndDesign