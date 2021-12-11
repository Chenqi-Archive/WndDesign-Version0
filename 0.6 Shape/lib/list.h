#pragma once

template<class T>
class list;

template<class T>
class listIter;

template<class T>
class listNode {
	listNode* next;
	T item;
	listNode(listNode* next, const T& item):next(next),item(item){}
	friend class list<T>;
	friend class listIter<T>;
};

template<class T>
class listIter {
	listNode<T>* node;

	friend class list<T>;
public:
	listIter<T>(listNode<T>* node) {
		this->node = node;
	}
	listIter<T>(const listIter<T>& iter) {
		this->node = iter.node;
	}
	listIter<T>& operator=(const listIter<T>& iter) {
		this->node = iter.node;
	}
	~listIter<T>() {
	}
	T& Item() const{
		return node->item;
	}
	void Next() {
		node = node->next;
	}
	void operator++() {
		Next();
	}
	operator bool() const{
		return node != nullptr;
	}
};

template<class T>
class list {
protected:
	int cnt;

	listNode<T>* head, * tail;

public:
	list<T>();
	list<T>(const list<T>& list);
	list<T>& operator=(const list<T>& list);
	~list();

	void clear();
	T& push_back(const T& item);
	void filter(list& list, bool (*filterProc)(const T& item));

	int size();

	// Insert an item before iter. Need to be double linked list
	bool insert(const listIter<T>& iter, const T& item);
	bool remove(listIter<T>& iter);

	listIter<T> begin();
};

template<class T>
inline list<T>::list()
{
	cnt = 0;
	head = tail = nullptr;
}

template<class T>
inline list<T>::list(const list<T>& list)
{
	cnt = 0;
	head = tail = nullptr;
	listNode<T>* p = list.head;
	while (p) {
		push_back(p->item);
		p = p->next;
	}
}

template<class T>
inline list<T>& list<T>::operator=(const list<T>& list)
{
	if (this == &list) {
		return *this;
	}
	clear();
	listNode<T>* p = list.head;
	while (p) {
		push_back(p->item);
		p = p->next;
	}
	return *this;
}

template<class T>
inline list<T>::~list()
{
	clear();
}

template<class T>
inline void list<T>::clear()
{
	listNode<T>* p = head;
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
inline T& list<T>::push_back(const T& item)
{
	listNode<T>* node = new listNode<T>(nullptr, item);
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
inline void list<T>::filter(list<T>& list, bool(*filterProc)(const T& item))
{
	if (this == &list) {
		cnt = 0;
		listNode<T>* prev = nullptr, * p = head, * tem;
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
		listNode<T>* p = list.head;
		while (p) {
			if (filterProc(p->item)) {
				push_back(p->item);
			}
			p = p->next;
		}
	}
}

template<class T>
inline int list<T>::size()
{
	return cnt;
}

template<class T>
inline bool list<T>::remove(listIter<T>& iter)
{
	if (!iter) {
		return false;
	}

	// Linear search. (May use double list instead)
	if (head == iter.node) {
		if (tail == iter.node) {
			tail = nullptr;
		}
		++iter;
		delete head; cnt--;
		head = iter.node;
		return true;
	}
	listNode<T>* p = head;
	while (p) {
		if (p->next == iter.node) {
			if (tail == iter.node) {
				tail = p;
			}
			++iter;
			delete p->next; cnt--;
			p->next = iter.node;
			return true;
		}
		p = p->next;
	}
	return false;
}

template<class T>
inline listIter<T> list<T>::begin()
{
	return head;
}
