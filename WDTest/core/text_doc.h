#pragma once

#include <WndDesign/sys/file.h>

#include <string>
#include <vector>

using namespace std;

using uint = unsigned int;
using wchar = wchar_t;

class TextDocLine {
	wstring line;

	// Dirty flag
	// Orignial size

public:
	void Insert(uint pos, const wstring& str);
	void Insert(uint pos, wchar ch);
	void Delete(uint pos, uint len);

	void* Read(void* p);
	void* Write(void* p);
};

class TextDoc {
	// File operations
private:
	File file;
public:
	bool NewFile();
	bool LoadFile(wstring fileName);
	bool SaveFile();

	// Edit
private:
	vector<TextDocLine> lines;

public:
	void Insert(uint lineNo, uint pos, const wstring& str);  // Caution! the str may contain \n
	void Insert(uint lineNo, uint pos, wchar ch);
	void Delete(uint lineNo, uint pos, uint len);
	void Delete(uint lineNo);
};