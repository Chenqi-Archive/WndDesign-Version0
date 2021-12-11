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
	matrix<T>(const matrix<T>& mat) {
		width = mat.width; height = mat.height;
		bits = new T[mat.width * mat.height];
		for (uint i = 0; i < width * height; ++i) {
			bits[i] = mat.bits[i];
		}
	}
	matrix<T>& operator=(const matrix<T>& mat) {
		if (&mat == this) { return *this; };
		if (bits == nullptr || bits != nullptr && width * height != mat.width * mat.height) {
			if (bits != nullptr) {
				delete[]bits;
			}
			bits = new T[mat.width * mat.height];
		}
		width = mat.width; height = mat.height;
		for (uint i = 0; i < width * height; ++i) {
			bits[i] = mat.bits[i];
		}
		return *this;
	}
	void Init(uint width, uint height, const T& item = 0) {
		this->width = width; this->height = height;
		bits = new T[width * height];
		for (uint i = 0; i < width * height; ++i) {
			bits[i] = item;
		}
	}
	void Init(Size size, const T& item = 0) { Init(size.width, size.height, item); }
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
	Size GetSize()const { return { width,height }; }
	~matrix() { if (bits != nullptr) { delete[]bits; } }
};