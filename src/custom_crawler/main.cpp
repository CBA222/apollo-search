#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include "crawler.h"

using namespace std;

int main(int argc, char *argv[]) {

	if (argc != 2) {
		cout << "Error!" << endl;
		return 1;
	}

	list<char *> seed_links;

	string line;
	ifstream input_file (argv[1]);
	if (input_file.is_open()) {
		while (getline(input_file, line)) {
			const char * line_chars = line.c_str();
			char* temp = (char *)malloc((strlen(line_chars) + 1) * sizeof(char));
			strcpy(temp, line_chars);
			seed_links.push_back(temp);
		}
		input_file.close();
	}

	cout << "Starting crawler." << endl;

	Crawler *my_crawler = new Crawler(seed_links);
	my_crawler->start_crawl();

	delete my_crawler;
	
	return 0;
}