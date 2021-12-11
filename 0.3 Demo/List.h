//
// List.h: singly linked list template
// 
// By Chenqi
// 

#pragma once

template<class T>
class List;

template<class T>
class ListIter;

template<class T>
class ListNode {
	ListNode* next;
	T item;

	friend class List<T>;
	friend class ListIter<T>;
};

template<class T>
class ListIter {
	ListNode<T>* node;

public:
	ListIter<T>(ListNode<T>* node) {
		this->node = node;
	}
	ListIter<T>(const ListIter<T>& iter) {
		this->node = iter.node;
	}
	ListIter<T>& operator=(const ListIter<T>& iter) {
		this->node = iter.node;
	}
	~ListIter<T>() {

	}
	T& Item() {
		return node->item;
	}
	void Next() {
		node = node->next;
	}
	void operator++() {
		Next();
	}
	operator bool() {
		return node != nullptr;
	}
};

template<class T>
class List {
protected:
	int cnt;

	ListNode<T>* head, * tail;

public:
	List<T>();
	List<T>(const List<T>& list);
	List<T>& operator=(const List<T>& list);
	~List();

	void clear();
	const T& push_back(const T& item);
	void filter(List& list, bool (*filterProc)(const T& item));

	int size();

	// Insert an item before iter. Need to be double linked list
	bool insert(const ListIter<T>& iter, const T& item);
	bool remove(const ListIter<T>& iter);

	ListIter<T> begin();
};

template<class T>
inline List<T>::List()
{
	cnt = 0;
	head = tail = nullptr;
}

template<class T>
inline List<T>::List(const List<T>& list)
{
	cnt = 0;
	head = tail = nullptr;
	ListNode<T>* p = list.head;
	while (p) {
		push_back(p->item);
		p = p->next;
	}
}

template<class T>
inline List<T>& List<T>::operator=(const List<T>& list)
{
	if (this == &list) {
		return *this;
	}
	clear();
	ListNode<T>* p = list.head;
	while (p) {
		push_back(p->item);
		p = p->next;
	}
	return *this;
}

template<class T>
inline List<T>::~List()
{
	clear();
}

template<class T>
inline void List<T>::clear()
{
	ListNode<T>* p = head;
	while (p) {
		head = head->next;
		delete p;
		p = head;
	}
	// Now head == NULL
	tail = nullptr;
	cnt = 0;
}

template<class T>
inline const T& List<T>::push_back(const T& item)
{
	ListNode<T>* node = new ListNode<T>;
	node->item = item;
	node->next = nullptr;
	if (cnt == 0) {
		head = tail = node;
	}
	else {
		tail->next = node;
		tail = node;
	}
	cnt++;

	return node->item;
}

template<class T>
inline void List<T>::filter(List<T>& list, bool(*filterProc)(const T& item))
{
	if (this == &list) {
		cnt = 0;
		ListNode<T>* prev = nullptr, * p = head, * tem;
		head = nullptr;
		while (p) {
			if (filterProc(p->item)) {
				if (prev == nullptr) {
					prev = p;
					head = prev;
				}
				else {
					prev->next = p;
					prev = prev->next;
				}
				p = p->next;
				cnt++;
			}
			else {
				tem = p->next;
				delete p;
				p = tem;
			}
		}
		if (prev) {
			prev->next = nullptr;
		}
		tail = prev;
	}
	else {
		clear();
		ListNode<T>* p = list.head;
		while (p) {
			if (filterProc(p->item)) {
				push_back(p->item);
			}
			p = p->next;
		}
	}
}

template<class T>
inline int List<T>::size()
{
	return cnt;
}

template<class T>
inline ListIter<T> List<T>::begin()
{
	return head;
}
