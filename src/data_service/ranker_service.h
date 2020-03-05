#include "ranker/pageranker.h"
#include "data_service/apollo_service.h"
#include "data_service/mq_handler.h"
#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>
#include <nlohmann/json.hpp>
//#include <yaml-cpp>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <unordered_map> 

using std::string;
using json = nlohmann::json;

struct SearchJob {
    const char* data;
    uint64_t data_size;
};

#ifndef RANKER_SERVICE_H
#define RANKER_SERVICE_H

class RankerService
{
private:
    string RABBITMQ_HOST;

    // Where to send messages back to client
    string EXCHANGE_NAME; 
    string ROUTING_KEY;
    // Where to recieve messages from client
    string REQUEST_QUEUE_NAME;
    // Where to recieve data from the the data service
    string DATA_QUEUE_NAME;
    
    int NUM_PAGES_TO_RETRIEVE;
    int MAX_JOB_BACKLOG;

    PageRanker ranker;
    std::queue<SearchJob> jobs_queue;
    std::shared_ptr<spdlog::logger> logger;
    AMQP::Channel channel = NULL;

    void jobs_loop();

public:
    RankerService();
    RankerService(string config_filepath);
    RankerService(std::unordered_map<string, string> config_map);
    ~RankerService();
    void start();
};

#endif