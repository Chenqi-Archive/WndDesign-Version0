#pragma once

#include "../types.h"

template<class T>
class matrix {
protected:
	uint width;
	uint height;
	T* bits;

public:
	matrix<T>() { width = 0; height = 0; bits = nullptr; }
	matrix<T>(uint width, uint height, const T& item = 0) {
		Init(width, height, item);
	}
	void Init(uint width, uint height, const T& item = 0) {
		this->width = width; this->height = height;
		bits = new T[width * height];
		for (uint i = 0; i < width * height; ++i) {
			bits[i] = item;
		}
	}
	// Set point(i,j), i is column number and j is row number.
	bool Set(uint i, uint j, const T& item) {
		if (i < width && j < height) {
			bits[j * width + i] = item;
			return true;
		}
		return false;
	}
	bool Get(uint i, uint j, T& pItem) const {
		if (i < width && j < height) {
			pItem = bits[j * width + i];
			return true;
		}
		return false;
	}
	~matrix() { if (bits != nullptr) { delete[]bits; } }
};