#include "fast_queue.h"

template <class T>
FastQueue::FastQueue() {
	elements = new deque<T>;
}

template <class T>
FastQueue::~FastQueue() {
	delete elements;
}

template <class T>
void FastQueue::insert(T elem) {
	elements->push_back(elem);
}

template <class T>
T FastQueue::remove(bool &success) {

	if (elements->empty()) {
		success = false;
		return NULL;
	}

	T elem = elements->front();
	elements->pop_front();

	success = true;
	return elem;
}

template <class T>
bool FastQueue::empty() {
	return elements->empty();
}