#pragma once
#include "../types.h"


#define INVALID_ITEM_ID (uint)-1

// Allocator: using an array to store pointers for elements already newed,
//   and don't need to explicitly delete them.
template<class T>
class allocator {
private:
	T** items;
	const uint maxSize;

	uint firstValidPos;
	uint lastUsedPos;

public:
	// Init allocator with type and max size(no more than (uint)-1)
	allocator<T>(uint maxSize);
	// Add a pointer to an already newed item, and return the item ID.
	bool addItem(T* pItem, uint& id);
	// Remove the item specified by id.
	bool removeItem(uint id);
	// Get the item specified by id. Pass a reference of the pointer.
	bool getItem(uint id, T*& pItem)const;
	// Return true if empty.
	bool empty()const;

	~allocator();
};

template<class T>
inline allocator<T>::allocator(uint maxSize): maxSize(maxSize){
	// May not need to allocate so much
	items = new T * [maxSize] {nullptr};
	firstValidPos = 0;
	lastUsedPos = INVALID_ITEM_ID;
}

template<class T>
inline bool allocator<T>::addItem(T* pItem, uint& id) {
	// No position avalible.
	if (firstValidPos >= maxSize) {
		return false;
	}
	
	// Return the item id.
	id = firstValidPos;

	// Store the pointer to the item.
	items[id] = pItem;
	
	// Update last tag position.
	if (id > lastUsedPos || lastUsedPos == INVALID_ITEM_ID) {
		lastUsedPos = id;
	}

	// Find next valid position. 
	// Could do some optimization by using a free list.
	do {
		firstValidPos++;
	} while (firstValidPos < maxSize && items[firstValidPos] != nullptr);

	return true;
}

template<class T>
inline bool allocator<T>::removeItem(uint id)
{
	if (id >= maxSize || items[id] == nullptr) {
		return false;
	}

	delete items[id];
	items[id] = nullptr;

	// Update firstValidPos.
	if (id < firstValidPos) {
		firstValidPos = id;
	}

	// Update lastUsedPos.
	while (lastUsedPos != INVALID_ITEM_ID && items[lastUsedPos] == nullptr) {
		lastUsedPos--;
	}

	return true;
}

template<class T>
inline bool allocator<T>::getItem(uint id, T*& pItem) const{
	if (id >= maxSize || items[id] == nullptr) {
		return false;
	}
	
	pItem = items[id];

	return true;
}

template<class T>
inline bool allocator<T>::empty() const
{
	return lastUsedPos == (uint)-1;
}

template<class T>
inline allocator<T>::~allocator(){
	while (lastUsedPos != INVALID_ITEM_ID) {
		if (items[lastUsedPos] != nullptr) {
			delete items[lastUsedPos];
			items[lastUsedPos] = nullptr;
		}
		lastUsedPos--;
	}	
	firstValidPos = 0;
	delete[]items;
}
