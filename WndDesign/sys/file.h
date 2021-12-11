#pragma once

#include <string>

using namespace std;

enum class FileOpenMode {
	// None,
	// WriteOnly,

	ReadOnly,      // Can not write to this file.
	ReadWrite,     // Can both read and write to this file.
};

enum class FileShareMode {
	Denied,        // Prevent other process from opening the file.
	ReadOnly,      // Allow other process to read the file.
	ReadWrite,     // Allow other process to read and write the file.

	// If the file has already been opened with certain share modes, then you must
	//   allow these modes as well, or the function fails.
};

enum class FileOpenAction {
	Normal,        // Open the file, if not exist, create a new file and open.  (OPEN_ALWAYS)
	MustExist,     // Open the existing file, if not exist, the function fails. (OPEN_EXISTING)
	MustNotExist,  // Create a new file, if already exist, the function fails.  (CREATE_NEW)

	// If you want to create a file and clear its content, use the Clear() function.
};

enum class FileAttribute {
	Normal,
	Hidden,
};

class File {
	using HANDLE = void*;
private:
	wstring file_name;
	size_t file_size;
	HANDLE hFile;
	HANDLE hFileMap;
	void* pFile;

public:
	File();
	~File();

public:
	bool Open(wstring fileName, 
			  FileOpenMode openMode = FileOpenMode::ReadOnly, 
			  FileShareMode shareMode = FileShareMode::ReadOnly,
			  FileOpenAction openAction = FileOpenAction::Normal);
	void Close();
	bool Resize(size_t size);
	bool SetAttribute(/**/);

public:
	const wstring& GetName() const { return file_name; }
	size_t GetSize() const { return file_size; }

public:
	bool Map(size_t pos, size_t size);
	const void* GetMapAddr() const;
	// size_t GetMapPos() const;
	// size_t GetMapSize() const;
	// bool Remap(pos, size);
};

wstring OpenFileDialog();