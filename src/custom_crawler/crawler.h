#include <map>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <deque>
#include <list>
#include <curl/curl.h>
#include <thread>
#include <vector>
#include "fast_hash.h"
#include "fast_queue.h"
using namespace std;

struct cmp_str
{
	bool operator()(char const *a, char const *b) const
	{
		return strcmp(a, b) < 0;
	}
};

class Crawler
{
private:
	int MAX_CONNECTIONS = 800;

	bool add_link(char* url);
	
public:
	list<char *> seed_links;
	int visit_count;

	FastHashSet *visited_links;

	FastQueue<char *> *link_queue;
	FastQueue<char *> *page_queue;

	Crawler(list<char *> sl);
	~Crawler();

	void traverse(char* url);
	void start_crawl();

	void find_links(char* content);
	void traverse_loop();

	void find_links_loop();
};