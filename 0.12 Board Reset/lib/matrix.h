#pragma once

#include "../types.h"

namespace WndDesign {

template<class T>
class matrix {
private:
	Size size;
	T* bits;
public:
	matrix<T>() :size(), bits(nullptr){}
	matrix<T>(Size size, const T& item = 0) {
		this->size = size;
		bits = new T[this->size.Area()];
		for (uint i = 0; i < this->size.Area(); ++i) {
			bits[i] = item;
		}
	}
	matrix<T>(const matrix<T>& mat) {
		size = mat.size;
		bits = new T[size.Area()];
		for (uint i = 0; i < size.Area(); ++i) {
			bits[i] = mat.bits[i];
		}
	}
	matrix<T>& operator=(const matrix<T>& mat) {
		if (&mat == this) { return *this; };
		if (bits == nullptr || (bits != nullptr && size.Area() != mat.size.Area())) {
			if (bits != nullptr) { delete[]bits; }
			bits = new T[mat.size.Area()];
		}
		size = mat.size;
		for (uint i = 0; i < size.Area(); ++i) {
			bits[i] = mat.bits[i];
		}
		return *this;
	}
	void Resize(Size size, const T& item = 0) {
		if (bits == nullptr || (bits != nullptr && this->size.Area() != size.Area())) {
			if (bits != nullptr) { delete[]bits; }
			bits = new T[size.Area()];
		}
		this->size = size;
		for (uint i = 0; i < this->size.Area(); ++i) {
			bits[i] = item;
		}
	}
	inline bool Set(Pos pos, const T& item) {
		if (pos.x < size.width && pos.y < size.height) { bits[pos.y * size.width + pos.x] = item; return true; }
		return false;
	}
	inline bool Get(Pos pos, T& item) const {
		if (pos.x < size.width && pos.y < size.height) { item = bits[pos.y * size.width + pos.x]; return true; }
		return false;
	}
	inline Size GetSize()const { return size; }
	inline const T* GetBits() const { return bits; }
	~matrix() { if (bits != nullptr) { delete[]bits; } }
};


} // namespace WndDesign