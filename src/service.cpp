#include "ranker/pageranker.h"
#include "search_index/index.h"
#include <iostream>
#include <list>
#include <fstream>
#include <string>


void populate_index_from_txt(Index *index, char* filepath);

int main(int argc, char* argv[]) {
    Index *index = new Index();
    PageRanker *ranker = new PageRanker(index);

    populate_index_from_txt(index, argv[1]);

    std::cout << "complete" << std::endl;
    delete index;
    delete ranker;
    return 0;
}

void populate_index_from_txt(Index *index, char* filepath) {

    std::list<char *> test_list;
	std::vector<char *> holding;

	std::string line;
	std::ifstream file(filepath);
	int idx = 0;
	if (file.is_open()) {
		while (getline(file, line)) {
			const char * line_chars = line.c_str();
			char* temp = (char *)malloc((strlen(line_chars) + 1) * sizeof(char));
			strcpy(temp, line_chars);

			//test_list.push_back(temp);
			holding.push_back(temp);
			idx += 1;

		}
		file.close();
	}

	index->populate(holding);
}

void exec_loop() {
    while (true) {
        
    }
}