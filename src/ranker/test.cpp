#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <ctime>

std::vector<int> generate_numbers(int num) {
	std::vector<int> mylist;

	for (int i = 0;i < num;i++) {
		mylist.push_back(rand() % num);
	}

	return mylist;
}

int main(int argc, char* argv[]) {

	std::vector<int> mylist = generate_numbers(90000000);
	std::cout << "BEGIN" << std::endl;

	auto start = std::chrono::system_clock::now();
	//sort(mylist.begin(), mylist.end());
	std::nth_element(mylist.begin(),mylist.begin()+ 10, mylist.end());
	mylist.resize(10);
	std::sort(mylist.begin(), mylist.end());

	// Some computation here
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end-start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

}