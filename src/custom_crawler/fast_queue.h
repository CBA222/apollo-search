#include <utility>
#include <deque>

using namespace std;


template <class T>
class FastQueue
{
private:
public:

	deque<T> *elements;

	FastQueue();
	~FastQueue();

	void insert(T elem);
	T remove(bool &success);

	bool empty();
};