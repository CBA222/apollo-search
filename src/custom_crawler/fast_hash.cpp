#include "fast_hash.h"

FastHashSet::FastHashSet() {
	elements = new unordered_set<char *>();
}

FastHashSet::~FastHashSet() {
	delete elements;
}

bool FastHashSet::put(char* key) {
	
	auto ret = elements->insert(key);

	return ret.second;
}