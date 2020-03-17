#include "data_service/mq/message_broker.h"

MessageBroker::MessageBroker() {

}

MessageBroker::~MessageBroker() {

}

void MessageBroker::start() {
    void *context = zmq_ctx_new();
    void *responder = zmq_socket(context, ZMQ_REP);
}