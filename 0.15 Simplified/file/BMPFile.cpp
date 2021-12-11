#include "BMPFile.h"
#include "../shape/Bitmap.h"
#include <cstring>

bool WndDesign::BMPFile::Open(const wchar_t* fileName) {
	if (!File::Open(fileName)) { return false; }
	do {
		if (Size().HighPart != 0) { break; }

		uint size = Size().LowPart;
		uint offset = 0;
		const char* const data = (char*)PFile();

		if (size - offset < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) { break; }

		fileHeader = (BITMAPFILEHEADER*)(data + offset);
		if (fileHeader->bfType.asWord != 0x4D42) { break; }  // "BM"
		if (size != fileHeader->bfSize) { break; }
		offset += sizeof(BITMAPFILEHEADER);
		
		infoHeader = (BITMAPINFOHEADER*)(data + offset);
		if (infoHeader->biHeight < 0) { break; }
		if (infoHeader->biPlanes != 1) { break; }
		if (infoHeader->biCompression != 0) { break; }
		offset += infoHeader->biSize;

		palette = (RGBQUAD*)(data + offset);

		bits = (void*)(data + fileHeader->bfOffBits);

		return true;
	} while (0);
//Error:
	Close();
	return false;
}

bool WndDesign::BMPFile::Create(const wchar_t* fileName, const Bitmap& bitmap, bool overwrite) {
	uint size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bitmap.GetSize().area() * 4;
	if (!File::Create(fileName, size, overwrite)) { return false; }

	uint offset = 0;
	const char* const data = (char*)PFile();

	fileHeader = (BITMAPFILEHEADER*)(data + offset);
	fileHeader->bfType.asWord = 0x4D42;  // "BM"
	fileHeader->bfSize = size;
	fileHeader->bfResevered1 = fileHeader->bfResevered2 = 0;
	offset += sizeof(BITMAPFILEHEADER);

	infoHeader = (BITMAPINFOHEADER*)(data + offset);
	infoHeader->biSize = sizeof(BITMAPINFOHEADER);
	infoHeader->biWidth = bitmap.GetSize().width;
	infoHeader->biHeight = bitmap.GetSize().height;
	infoHeader->biPlanes = 1;
	infoHeader->biBitCount = 32;
	infoHeader->biCompression = 0;
	infoHeader->biSizeImage = 0;
	infoHeader->biXPelsPerMeter = 0;
	infoHeader->biYPelsPerMeter = 0;
	infoHeader->biClrUsed = 0;
	infoHeader->biClrImportant = 0;
	offset += sizeof(BITMAPINFOHEADER);

	palette = nullptr;

	fileHeader->bfOffBits = offset;
	bits = (void*)(data + offset);
	for (uint j = 0; j < bitmap.GetSize().height; ++j) {
		memcpy((Color32*)bits + (bitmap.GetSize().height - j - 1) * bitmap.GetSize().width,
			   (Color32*)bitmap.GetBits() + j * bitmap.GetSize().width,
			   bitmap.GetSize().width * sizeof(Color32));
	}

	return true;
}


bool WndDesign::BMPFile::Read(Bitmap& bitmap) const {
	if (bits == nullptr) { bitmap = Bitmap(); return false; }

	WndDesign::Size size = { infoHeader->biWidth, infoHeader->biHeight };
	bitmap.Resize(size);

	uint lineSize;  // bytes
	switch (infoHeader->biBitCount) {
		//case 1:
		//case 4:
		//case 8:
		//case 16:
		case 24:		
			lineSize = DivideCeil(size.width * 3, 4) * 4;
			for (uint j = 0; j < size.height; ++j) {
				for (uint i = 0; i < size.width; ++i) {
					RGBTRIPLE color24 = *(RGBTRIPLE*)((char*)bits + (size.height - j - 1) * lineSize + i * 3);
					Color32 color((BYTE)0xFF, color24.rgbRed, color24.rgbGreen, color24.rgbBlue);
					bitmap.Set({ i, j }, color);
				}
			}
			break;
		case 32:		
			lineSize = size.width * 4;
			for (uint j = 0; j < size.height; ++j) {
				memcpy((Color32*)bitmap.GetBits() + j * bitmap.GetSize().width,
					   (Color32*)bits + (bitmap.GetSize().height - j - 1) * bitmap.GetSize().width,
					   bitmap.GetSize().width * sizeof(Color32));
			}
			break;
		default:return false;
	}
	return true;
}

void WndDesign::BMPFile::Close() {
	fileHeader = nullptr;
	infoHeader = nullptr;
	palette = nullptr;
	bits = nullptr;
	File::Close();
}
