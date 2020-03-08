#include "ranker/pageranker.h"
#include "data_service/apollo_service.h"
#include "data_service/mq_handler.h"
#include "data_service/service_utils.h"
#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>
#include <nlohmann/json.hpp>
//#include <yaml-cpp>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <unordered_map> 
#include <condition_variable>
#include <toml11/toml.hpp>
#include <iostream>


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
    std::mutex startup_mx;
    std::condition_variable startup_cv;

    struct MQ_MESSAGE setup_message;

    string RABBITMQ_HOST;

    struct CONNINFO SETUP_CONN;
    struct CONNINFO REQUEST_CONN;
    struct CONNINFO DATA_CONN;
    
    int NUM_PAGES_TO_RETRIEVE;
    int MAX_JOB_BACKLOG;

    PageRanker ranker;
    std::queue<SearchJob> *jobs_queue;
    AMQP::TcpChannel *channel;
    stdout_logger *logger;

    void process_startup_message(MQ_MESSAGE);
    void jobs_loop();
    void send_message();

public:
    RankerService();
    RankerService(string config_filepath);
    RankerService(std::unordered_map<string, string> config_map);
    ~RankerService();
    void start();
};

#endif