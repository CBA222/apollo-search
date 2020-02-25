#include "index.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include "timer.h"
#include <vector>

int main(int argc, char* argv[]) {
	Index *test_index = new Index();

	list<char *> test_list;

	std::vector<char *> holding;

	string line;
	ifstream file(argv[1]);
	int idx = 0;
	if (file.is_open()) {
		while (getline(file, line)) {
			const char * line_chars = line.c_str();
			char* temp = (char *)malloc((strlen(line_chars) + 1) * sizeof(char));
			strcpy(temp, line_chars);

			//test_index->insert(temp, 0);
			//free(temp);
			test_list.push_back(temp);
			holding.push_back(temp);
			idx += 1;

		}
		file.close();
	}


	test_index->populate(holding);

	int size = 5;
	uint64 prev = GetTimeMs64();
	for (int i = 0;i < test_list.size();i++) {
		//int* table = test_index->get("HELLO", size);
		char* temp = test_list.front();
		test_list.pop_front();
		int* table = test_index->get(temp, size);
	}
	
	uint64 now = GetTimeMs64();
	cout << now - prev << endl;
	/*
	cout << "Keys inserted: " << test_index->get_key_count() << endl;
	cout << "Collisions: " << test_index->get_collisions() << endl;

	
	int size = 5;
	uint64 prev = GetTimeMs64();
	for (int i = 0;i < test_list.size();i++) {
		//int* table = test_index->get("HELLO", size);
		char* temp = test_list.front();
		test_list.pop_front();
		int* table = test_index->get(temp, size);
	}
	
	uint64 now = GetTimeMs64();
	cout << now - prev << endl;
	cout << size << endl;
	*/
	delete test_index;

}