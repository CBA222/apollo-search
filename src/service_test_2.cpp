#include "data_service/ranker_service2.h"
#include <iostream>

int main(int argc, char* argv[]) {
    
    RankerService *ranker = new RankerService();
    ranker->configure("tcp://127.0.0.1:1003");
    ranker->start();
}