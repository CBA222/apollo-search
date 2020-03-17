#include <string>
#include <thread>
#include <condition_variable>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include "data_service/mq/deferred_consumer.h"
#include "data_service/mq/message.h"
#include <signal.h>
#include <iostream>
#include "logging/stdout_logger.h"

using namespace std;

namespace MQ {

class Channel
{
private:
    stdout_logger *logger;
    DeferredConsumer *tracker;
    std::thread consume_thread_;
    bool consuming = false;
    string address;

    zmq::context_t context;
    zmq::socket_t socket;
    zmq::socket_t controller;

    std::mutex sig_mtx;
    bool close_thread = false;

    void consume_loop();
    //void (*consume_callback)(Message) = NULL;
    std::function<void(Message)> consume_callback;
public:
    Channel(string);
    ~Channel();

    Channel& publish(string);
    Channel& consume();
    Channel& onReceived(std::function<void(Message)>);

    Message publish_blocking(string);
    void close();
};

}