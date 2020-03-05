#include "index.h"

Index::Index() {
}

Index::~Index() {
	for (int i = 0;i < ROW_COUNT;i++) {
		delete[] table[i];
	}

	delete[] table;
	delete[] col_count;

	for (int i = 0;i < ROW_COUNT;i++) {
		delete[] keys[i];
	}
	delete[] keys;
	//delete bphf;
}

void Index::populate(std::vector<char *> keys_in) {

	int num_keys = keys_in.size();
	ROW_COUNT = num_keys;

	table = new int*[ROW_COUNT];

	for (int i = 0;i < ROW_COUNT;i++) {
		table[i] = new int[COL_COUNT];
		//table[i] = NULL;
	}

	col_count = new int[ROW_COUNT];

	for (int i = 0;i < ROW_COUNT;i++) {
		col_count[i] = 0;
	}

	keys = new char*[ROW_COUNT];

	for (int i = 0;i < ROW_COUNT;i++) {
		keys[i] = NULL;
	}

	//bphf = new boomphf::mphf<char *,Custom_string_Hasher>(keys_in.size(), keys_in, 1, 4.0);

	for (std::vector<char *>::iterator it=keys_in.begin();it!=keys_in.end();it++) {
		insert(*it, 0);
		free(*it);
	}
}

bool Index::insert(char* key, int val) {
	bool in_set;
	unsigned int idx = get_idx(key, in_set);

	if (!in_set) {
		return false;
	}

	int c_count = col_count[idx];
	if (c_count != 0) {
		keys[idx] = key;
	} else {
		key_count += 1;
	}

	keys[idx] = new char[strlen(key) + 1];
	strcpy(keys[idx], key);

	table[idx][c_count] = val;
	col_count[idx] += 1;


	return true;
}

bool Index::insert(char* key, int *vals, int vals_size) {
	bool in_set;
	unsigned int idx = get_idx(key, in_set);

	table[idx] = new int[vals_size];
	for (int i = 0;i < vals_size;i++) {
		table[idx][i] = vals[i];
	}
	col_count[idx] = vals_size;

	return true;
}

int* Index::get(char* key, int& size) {
	bool in_set;
	unsigned int idx = get_idx(key, in_set);
	if (!in_set) {
		return NULL;
	}
	//cout << "GET " << key << " IDX " << idx << endl;
	size = col_count[idx];
	return table[idx];
}

std::vector<int> Index::get_intersection(char** keys, unsigned int num_keys) {

	//order keys
	int init_size;
	int* init_ray = get(keys[0], init_size);

	std::vector<int> intersected;
	intersected.assign(init_ray, init_ray + init_size);

	for (int i = 0;i < num_keys - 1;i++) {
		int size_1 = intersected.size();
		int size_2;

		int* ray_1 = intersected.data();
		int* ray_2 = get(keys[i + 1], size_2);

		int pos_1 = 0; int pos_2 = 0;
		int val_1, val_2;

		intersected.clear();

		while (pos_1 < size_1 && pos_2 < size_2) {
			val_1 = ray_1[pos_1];
			val_2 = ray_2[pos_2];

			if (val_1 < val_2) {
				pos_1 += 1;
			} else if (val_1 > val_2) {
				pos_2 += 1;
			} else {
				intersected.push_back(val_1);
				pos_1 += 1;
				pos_2 += 1;
			}
		}
	}

	return intersected;

	/*
	Go through each key
	For each key, generate new hash table. Add all keys that exist in previous hash table.
	Repeat.

	*/
	/*
	int size;
	
	int* first = get(keys[0], size);
	FastHashSet<int> *hash_table = new FastHashSet<int>();

	for (int i = 0;i < size;i++) {
		hash_table->put(first[i]);
	}

	list<int> final;

	for (int i = 1;i < num_keys;i++) {

		int* to_comp = get(keys[i], size);
		FastHashSet<int> *new_hash_table = new FastHashSet<int>();

		for (int j = 0;j < size;i++) {
			int to_check = to_comp[j];
			bool already_in = hash_table->get(to_check);

			if (already_in) {
				new_hash_table->put(to_check);

				if (i == num_keys - 1) {
					final.push_back(num_keys);
				}
			}
		}

		delete hash_table;
		hash_table = new_hash_table;
	}


	for (list<int> interator i = final.begin();i != final.end();i++) {

	}
	*/
	

}

unsigned int Index::get_idx(char* key, bool& in_set) {
	/*
	uint64_t idx = bphf->lookup(key);
	in_set = true;
	if (idx == ULLONG_MAX) {
		in_set = false;
	}
	return idx;
	*/
	
	in_set = true;
	unsigned int base_idx = hash_func(key) % ROW_COUNT;

	if (keys[base_idx] == NULL) {
		//cout << "NULL " << endl;
		return base_idx;
	}

	if (strcmp(key, keys[base_idx]) == 0) {
		return base_idx;
	} else {
		collisions += 1;
	}
	
	unsigned int new_idx = base_idx;

	char* to_comp;

	do {
		new_idx += 1;
		if (new_idx == ROW_COUNT) {
			new_idx = 0;
		}
		to_comp = keys[new_idx];

		if (to_comp == NULL) {
			return new_idx;
		}
		if (strcmp(key, to_comp) == 0) {
			return new_idx;
		}

	} while (new_idx != base_idx);

	in_set = false;

	return 0;
	
}

unsigned int Index::hash_func(char* str) {
	//hash<string_view> ptr_hash;
	//return ptr_hash(key);

	unsigned int offset = UINT64_C(14695981039346656037);
    unsigned int prime = UINT64_C(1099511628211);

	unsigned long hash = offset;
	int c;

	while (c = *str++) {
		hash = (hash ^ c) * prime;
	}

	return hash;

}

int Index::get_key_count() {
	return key_count;
}

int Index::get_collisions() {
	return collisions;
}