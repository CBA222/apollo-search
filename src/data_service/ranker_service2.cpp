#include "data_service/ranker_service2.h"

static int s_interrupted = 0;
static void s_signal_handler(int sv) {
    s_interrupted = 1;
}

static void s_catch_signals() {
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}

RankerService::RankerService() {
    logger = new stdout_logger();
}

void RankerService::configure(string s_h) {
    socket_host = s_h;
}

vector<Page> get_documents(std::vector<int> doc_ids) {

};

void RankerService::start() {

    s_catch_signals();

    ranker = new PageRanker();
    
    MQ::Channel channel(socket_host);

    channel.onReceived([&channel, this](MQ::Message msg) {

        struct SearchQuery query;
        query.deserialize(msg.data, msg.size);

        logger->INFO("Query received: " + (string)query.free_text);

        std::vector<PageResult> results = ranker->rank(query.free_text, 10);

        struct SearchResults results_buf;
        results_buf.page_results = results;

        channel.publish(results_buf.serialize());

    });

    channel.consume();

    while (1) {
        if (s_interrupted == 1) {
            channel.close();
            break;
        }
    }
}