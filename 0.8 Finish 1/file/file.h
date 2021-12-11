#pragma once
#pragma pack(1)

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef long                LONG;


// Use memory map!
class File {
protected:
    HANDLE hFile;
    HANDLE hFileMap;
    PVOID pvFile;
    LARGE_INTEGER size;
public:
    File();
    bool OpenFile(const wchar_t* fileName);
    void CloseFile();
    ~File();
};


class BMPfile :public File {
    struct BITMAPFILEHEADER {           // 0x00
        WORD    bfType;                 // 0x02
        DWORD   bfSize;                 // 0x06
        WORD    bfReserved1;            // 0x08
        WORD    bfReserved2;            // 0x0A
        DWORD   bfOffBits;              // 0x0E
    };
    struct BITMAPINFOHEADER {           // 0x00
        DWORD      biSize;              // 0x04
        LONG       biWidth;             // 0x08
        LONG       biHeight;            // 0x0C
        WORD       biPlanes;            // 0x0E
        WORD       biBitCount;          // 0x10
        DWORD      biCompression;       // 0x14
        DWORD      biSizeImage;         // 0x18
        LONG       biXPelsPerMeter;     // 0x1C
        LONG       biYPelsPerMeter;     // 0x20
        DWORD      biClrUsed;           // 0x24
        DWORD      biClrImportant;      // 0x28
    };
    struct RGBQUAD {                    // 0x00
        BYTE    rgbBlue;                // 0x01
        BYTE    rgbGreen;               // 0x02
        BYTE    rgbRed;                 // 0x03
        BYTE    rgbReserved;            // 0x04
    };

private:
    BITMAPFILEHEADER *fileHeader;
    BITMAPINFOHEADER *infoHeader;
    RGBQUAD* rgbQuad;
    void* data;

public:
    bool OpenFile(const wchar_t* fileName);

};



class TTFfile:public File {
	
};