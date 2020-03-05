/*
High-performance in-memory inverted index
*/

#include <functional>
#include <algorithm>
#include <iostream>
#include <string>
#include <string.h>
#include <exception>
#include <vector>
//#include "BooPHF.h"

#ifndef INDEX_H
#define INDEX_H

/*
class Custom_string_Hasher
{
public:
	// the class should have operator () with this signature :
	uint64_t operator ()   (char *str, uint64_t seed=0) const
	{
		
		uint64_t offset = UINT64_C(14695981039346656037);
	        uint64_t prime = UINT64_C(1099511628211);

		uint64_t hash = offset;
	        int c;

	        while (c = *str++) {
	        	hash = (hash ^ c) * prime;
	        }

		hash ^= seed;
		
		return hash;
	}

};*/

//typedef boomphf::mphf<  char *, Custom_string_Hasher  > boophf_t;

class Index
{
private:

	int ROW_COUNT = 100;
	int COL_COUNT = 1;
	int **table;
	int *col_count;
	char** keys;

	//boophf_t * bphf = NULL;

	int key_count = 0;
	int collisions = 0;
	
	unsigned int hash_func(char* str);

public:
	
	Index();
	~Index();

	unsigned int get_idx(char* key, bool& in_set);

	void populate(std::vector<char *> keys);

	bool insert(char* key, int val);
	bool insert(char* key, int *vals, int vals_size);
	int* get(char* key, int& size);

	int get_key_count();
	int get_collisions();

	std::vector<int> get_intersection(char** keys, unsigned int size);

};

#endif