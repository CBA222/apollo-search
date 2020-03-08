#include "data_service/document_service.h"

SearchDataService::SearchDataService() {
    RABBITMQ_HOST = "rabbit@LAPTOP-O75GHEAB";
    REQUEST_CONN.EXCHANGE_NAME = "apollo_exchange";
    REQUEST_CONN.QUEUE_NAME = "data_requests";
    REQUEST_CONN.ROUTING_KEY = "data_requests";

    logger = new stdout_logger();
}

SearchDataService::~SearchDataService() {
    
}

void SearchDataService::start() {
    AppTcpHandler handler;
    AMQP::Address address(RABBITMQ_HOST);
    AMQP::TcpConnection connection(&handler, address);
    channel = new AMQP::TcpChannel(&connection);

    channel->consume(REQUEST_CONN.QUEUE_NAME)
        .onReceived([this](const AMQP::Message &message, uint64_t delivery_tag, bool redelivered) {

        });

    /*
    channel->consume(REQUEST_CONN.QUEUE_NAME)
        .onSuccess([this](const std::string &consumer_tag) {
            logger->INFO("Starting consumption of startup message.");
        })
        .onReceived([this](const AMQP::Message &message, uint64_t delivery_tag, bool redelivered) {
            logger->INFO("Recieved startup message");
        })
        .onError([this](const char* message) {
            logger->INFO("Error in consumption of startup message.");
        });*/

    logger->INFO("Server Started.");
    while (true) {

    }
}