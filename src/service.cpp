#include "ranker/pageranker.h"
#include "search_index/index.h"
#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <event2/event.h>


int main(int argc, char* argv[]) {
    Index *index = new Index();
    //PageRanker *ranker = new PageRanker(index);

    std::cout << "complete" << std::endl;
    delete index;
    //delete ranker;
    return 0;
}