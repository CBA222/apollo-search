#include "data_service/ranker_service.h"

RankerService::RankerService() {}

RankerService::RankerService(string config_filepath) {
    //YAML::Node config_file = YAML::LoadFile(config_filepath);
    //RABBITMQ_HOST = config_file['rabbitmq']['host'];
}

RankerService::RankerService(std::unordered_map<string, string> config_map) {
    RABBITMQ_HOST = config_map.at("rabbitmq_host");
}

RankerService::~RankerService() {

}

void RankerService::start() {
    AppTcpHandler handler;
    AMQP::Address address(RABBITMQ_HOST);
    AMQP::TcpConnection connection(&handler, address);
    channel = AMQP::Channel(&connection);

    //logger = spdlog::stdout_color_mt("main_logger");

    auto messageCB = [&channel, this](const AMQP::Message &message, uint64_t delivery_tag, bool redelivered) {
        if (jobs_queue.size() < MAX_JOB_BACKLOG) {
            struct SearchJob job {message.body(), message.bodySize()};
            jobs_queue.push(job);
            channel.ack(delivery_tag);
            //logger->info("JOB PUSHED.");
        } else {
            //logger->info("JOB QUEUE FULL. REJECTING MESSAGE.");
        }
    };

    auto errorCB = []() {
        //logger->error("ERROR RECIEVING MQ MESSAGE.");
    };

    auto receivedCB = []() {
        //logger->error("ERROR RECIEVING MQ MESSAGE.");
    };

    channel.consume(REQUEST_QUEUE_NAME)
        .onReceived(receivedCB)
        .onSuccess(messageCB)
        .onError(errorCB);

    jobs_loop();
}

void RankerService::jobs_loop() {
    while (true) {
        if (jobs_queue.size() > 0) {
            struct SearchJob job = jobs_queue.back();

            string msg_str(job.data);
            json query_data = json::parse(msg_str);

            std::vector<const char *> tokens;
            for (auto it = query_data.begin();it != query_data.end();it++) {
                tokens.push_back(it.value());
            }
            //std::vector<char *> tokens = query_data["tokens"];
            std::vector<PageR> ranked_pages = ranker.rank(tokens, NUM_PAGES_TO_RETRIEVE);

            json results_data;
            results_data["id"] = query_data["id"];
            results_data["results"] = new std::vector<char *>(NUM_PAGES_TO_RETRIEVE);
            channel.publish(EXCHANGE_NAME, ROUTING_KEY, results_data.dump());

            jobs_queue.pop();
            //logger->info("JOB COMPLETED.");
        }
    }
}