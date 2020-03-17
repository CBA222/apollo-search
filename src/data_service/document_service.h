#include <string>
#include <vector>
#include <queue>
#include <zmq.hpp>
#include <zmq_addon.hpp>
#include "data_service/apollo_service.h"
#include "logging/stdout_logger.h"
#include "data_service/mq/channel.h"
#include "data_service/mq/message.h"
#include "ranker/ranker_utils.h"
#include <nlohmann/json.hpp>
#include <libpq-fe.h>

using json = nlohmann::json;

using std::string;

#ifndef DOCUMENT_SERVICE_H
#define DOCUMENT_SERVICE_H

class SearchDataService : public ApolloService
{
private:
    stdout_logger *logger;
    string socket_host;
    string sql_host;

    MQ::Channel *setup_channel;
    MQ::Channel *data_channel;

    PGconn *conn;

    vector<struct Page> get_documents(vector<int> doc_ids);
public:
    SearchDataService();
    ~SearchDataService();
    void start();

    void configure(
        string socket_host,
        string sql_param_vals
    );
};

#endif