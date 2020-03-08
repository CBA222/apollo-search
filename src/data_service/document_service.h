#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <queue>
#include "data_service/apollo_service.h"
#include "data_service/service_utils.h"
#include "data_service/mq_handler.h"
#include "logging/stdout_logger.h"

using std::string;

#ifndef DOCUMENT_SERVICE_H
#define DOCUMENT_SERVICE_H

class SearchDataService : public ApolloService
{
private:
    string RABBITMQ_HOST;
    struct CONNINFO REQUEST_CONN;

    AMQP::TcpChannel *channel;
    stdout_logger *logger;
public:
    SearchDataService();
    ~SearchDataService();
    void start();
};

#endif