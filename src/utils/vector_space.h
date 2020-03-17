#include <string>
#include <string.h>
#include <vector>

#ifndef VECTOR_SPACE_H
#define VECTOR_SPACE_H

class VectorSpace {
private:
    char** keys;
    int ROW_COUNT;

    unsigned int hash_func(char* str) {
        unsigned int offset = UINT64_C(14695981039346656037);
        unsigned int prime = UINT64_C(1099511628211);

        unsigned long hash = offset;
        int c;

        while (c = *str++) {
            hash = (hash ^ c) * prime;
        }

        return hash;
    }
public:
    VectorSpace() {};
    /* 
     * NO DUPLICATE CHECKS
     * 
     * 
     */
    VectorSpace(std::vector<char *> tokens) {
        int num_keys = tokens.size();
        ROW_COUNT = num_keys;

        keys = new char*[ROW_COUNT];
        for (int i = 0;i < ROW_COUNT;i++) {
            keys[i] = NULL;
        }

        for (auto it=tokens.begin();it!=tokens.end();it++) {
            char* key = *it;
            unsigned int base_idx = hash_func(key) % ROW_COUNT;
            unsigned int new_idx = base_idx;

            do {
                if (keys[new_idx] == NULL) {
                    keys[new_idx] = new char[strlen(key) + 1];
	                strcpy(keys[new_idx], key);
                    break;
                }

                new_idx += 1;
                if (new_idx == ROW_COUNT) {
                    new_idx = 0;
                }

            } while (new_idx != base_idx);
        }
    }

    ~VectorSpace() {
        for (int i = 0;i < ROW_COUNT;i++) {
            delete[] keys[i];
        }
        delete[] keys;
    }

    unsigned int get_idx(char* key, bool& in_set) {
        in_set = true;
        unsigned int base_idx = hash_func(key) % ROW_COUNT;
        unsigned int new_idx = base_idx;

        do {
            if (strcmp(key, keys[new_idx]) == 0) {
                return new_idx;
            }

            new_idx += 1;
            if (new_idx == ROW_COUNT) {
                new_idx = 0;
            }

        } while (new_idx != base_idx);

        in_set = false;
        return 0;
    }
};

#endif