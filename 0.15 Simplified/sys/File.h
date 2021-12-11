#pragma once

#include "../basic/type.h"

namespace WndDesign {

typedef char                CHAR;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef int                 BOOL;
typedef long                LONG;
typedef __int64             LONGLONG;

typedef void* HANDLE;
typedef void* PVOID;
typedef union _LARGE_INTEGER {
    struct { DWORD LowPart; LONG HighPart; };
    LONGLONG QuadPart;
} LARGE_INTEGER;


class File {
private:
    HANDLE hFile;
    HANDLE hFileMap;
    PVOID pFile;
    LARGE_INTEGER size;
protected:
    File();
    // Open an existing file (read only).
    bool Open(const wchar_t* fileName);
    // Edit an existing file (read and write).
    bool Edit(const wchar_t* fileName);
    // Create a new file with fixed size (write).
    bool Create(const wchar_t* fileName, uint size, bool overwrite = false);
    void Close();
    inline PVOID PFile() const { return pFile; }
    ~File();
public:
    inline const LARGE_INTEGER& Size() const { return size; }
};


bool OpenFileDialog(wchar_t* fileName, uint length);

bool OpenFolderDialog(wchar_t* folderName, uint length);

} // namespace WndDesign