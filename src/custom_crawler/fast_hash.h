#include <unordered_set>
#include <iterator>
#include <stdio.h>

using namespace std;

class FastHashSet
{
private:
	
public:
	FastHashSet();
	~FastHashSet();

	unordered_set<char *> *elements;
	bool put(char* key);
	bool get(char* key);
};