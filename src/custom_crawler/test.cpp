#include <thread>
#include <iostream>

using namespace std;

void foo(int &a) {
	a += 1;
}

int main(int argc, char* argv[]) {
	cout << thread::hardware_concurrency() << endl;
	int b = 1;
	cout << b << endl;
	foo(b);
	cout << b << endl;
}