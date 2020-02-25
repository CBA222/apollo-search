//#include "BooPHFall.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <random>
#include <algorithm>
#include <fstream>

#include "index.h"

using namespace std;

/*

//example with user provided custom hasher for uint64_t type :

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

};


//then tell BBhash to use this custom hash : (also appears below, line 104)
typedef boomphf::mphf<  char *, Custom_string_Hasher  > boophf_t;



//from http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
void gen_random(char *s, const int len) {
	static const char alphanum[] =
	"0123456789"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz";
	
	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}
	
	s[len] = 0;
}

std::vector<int> generate_numbers(int num) {
	std::vector<int> mylist;

	for (int i = 0;i < num;i++) {
		mylist.push_back(rand() % num);
	}

	return mylist;
}
*/

Index* create_test_index() {
	Index *test_index = new Index();
	vector<char *> keys;
	keys.push_back("a");
	keys.push_back("b");
	keys.push_back("c");
	keys.push_back("d");
	keys.push_back("e");
	test_index->populate(keys);

	vector<int> a_vals;
	a_vals.push_back(1);
	a_vals.push_back(2);
	a_vals.push_back(3);
	a_vals.push_back(4);
	a_vals.push_back(5);
	a_vals.push_back(6);
	a_vals.push_back(7);
	a_vals.push_back(8);

	vector<int> b_vals;
	b_vals.push_back(2);
	b_vals.push_back(3);
	b_vals.push_back(4);
	b_vals.push_back(5);
	b_vals.push_back(6);
	b_vals.push_back(7);
	b_vals.push_back(8);
	b_vals.push_back(9);

	vector<int> c_vals;
	c_vals.push_back(2);
	c_vals.push_back(3);
	c_vals.push_back(4);
	c_vals.push_back(10);
	
	test_index->insert("a", a_vals.data(), a_vals.size());
	test_index->insert("b", b_vals.data(), b_vals.size());
	test_index->insert("c", c_vals.data(), c_vals.size());
	return test_index;
}

int test_get(Index* test_index) {
	int size;
	int* vals = test_index->get("a", size);

	for (int i = 0;i < size;i++) {
		cout << vals[i] << endl;
	}
}

int test_intersection(Index* test_index) {
	vector<char *> chars;
	chars.push_back("a");
	chars.push_back("b");
	chars.push_back("c");
	vector<int> intersection = test_index->get_intersection(chars.data(), chars.size());

	for (vector<int>::iterator it=intersection.begin();it!=intersection.end();it++) {
		cout << *it << " ";
	}
	cout << endl;
	return 0;
}

int test_bits() {

	uint64_t a = 2379811;

	uint8_t* encoded_ray = new uint8_t[9];

	for (int i = 0;i < 9;i++) {
		encoded_ray[i] = 0;
		encoded_ray[i] = ((1 << 7) - 1) & (a >> (7 * i));
		//cout << (uint64_t) encoded_ray[i] << endl;
		if (i == 8) {
			//encoded_ray[i] = 128 | encoded_ray[i];
		}
	}

	uint64_t b = 0;

	for (int i = 0;i < 9;i++) {
		uint64_t leading_bit = 128 & encoded_ray[i];
		cout << leading_bit << endl;
		uint64_t temp = encoded_ray[i] << (i * 7);
		b = b | temp;
	}

	cout << b << endl;

	return 0;
}


int main (int argc, char* argv[]){

	//test_bits();
	Index* test_index = create_test_index();
	//test_get(test_index);
	test_intersection(test_index);

	/*
	
	uint nthreads = 1;
	
	uint64_t ii, jj;
	std::vector<char *> data;

	int string_size = 20;

	data.push_back("brian");
	data.push_back("michael");

	boophf_t * bphf = NULL;
	bphf = new boomphf::mphf<char *,Custom_string_Hasher>(data.size(), data, nthreads);

	for (u_int64_t i = 0; i < data.size(); i++){
		uint64_t  idx = bphf->lookup(data[i]);
		cout << idx << endl;
	}
	
	delete bphf;
	return EXIT_SUCCESS;
	*/
}