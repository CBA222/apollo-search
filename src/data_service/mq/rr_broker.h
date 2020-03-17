#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <queue>
#include <string>
#include <iostream>
#include <signal.h>
#include "logging/stdout_logger.h"
#include <fmt/format.h>

using std::string;

/*
 * Request Reply Broker
 */
class RRBroker
{
private:
    std::queue<string> worker_queue;
    stdout_logger *logger;

    string frontend_host;
    string backend_host;
public:
    RRBroker(string, string);
    void start();
};