#include "data_service/ranker_service.h"

RankerService::RankerService() {
    logger = new stdout_logger();
    logger->INFO("RANKER SERVICE STARTED");
}

RankerService::RankerService(string config_filepath) {
    logger = new stdout_logger();
    auto data = toml::parse(config_filepath);

    auto rabbitmq = toml::find(data, "rabbitmq");
    RABBITMQ_HOST = toml::find<string>(rabbitmq, "host");

    auto setup_conn_info = toml::find(rabbitmq, "setup_conn");
    SETUP_CONN.QUEUE_NAME = toml::find<string>(setup_conn_info, "queue_name");
    SETUP_CONN.EXCHANGE_NAME = toml::find<string>(setup_conn_info, "exchange_name");
    SETUP_CONN.ROUTING_KEY = toml::find<string>(setup_conn_info, "routing_key");

    auto request_conn_info = toml::find(rabbitmq, "request_conn");
    REQUEST_CONN.QUEUE_NAME = toml::find<string>(request_conn_info, "queue_name");
    REQUEST_CONN.EXCHANGE_NAME = toml::find<string>(request_conn_info, "exchange_name");
    REQUEST_CONN.ROUTING_KEY = toml::find<string>(request_conn_info, "routing_key");

    auto data_conn_info = toml::find(rabbitmq, "data_conn");
    DATA_CONN.QUEUE_NAME = toml::find<string>(data_conn_info, "queue_name");
    DATA_CONN.EXCHANGE_NAME = toml::find<string>(data_conn_info, "exchange_name");
    DATA_CONN.ROUTING_KEY = toml::find<string>(data_conn_info, "routing_key");

    logger->INFO("RANKER SERVICE STARTED");
}

RankerService::RankerService(std::unordered_map<string, string> config_map) {
}

RankerService::~RankerService() {

}

void RankerService::send_message() {
    //channel.publish();
}

void RankerService::process_startup_message(MQ_MESSAGE msg) {
    string msg_str(msg.data, msg.data_size);
    json json_data(msg_str);

    for (auto& element : json_data["inverted_index"]) {

    }
}

void RankerService::start() {
    /*
     *
     *
     *
     *
     *
     *
     *
     */

    AppTcpHandler handler;
    AMQP::Address address(RABBITMQ_HOST);
    AMQP::TcpConnection connection(&handler, address);
    channel = new AMQP::TcpChannel(&connection);

    /* 
     * SETUP: Get setup data from the server
     * - Inverted index
     * - Vector space mapping
     */

    json startup_message;
    startup_message["id"] = "RANKER_SERVICE";
    channel->publish(SETUP_CONN.EXCHANGE_NAME, SETUP_CONN.ROUTING_KEY, startup_message.dump());

    channel->consume(SETUP_CONN.QUEUE_NAME)
        .onSuccess([this](const std::string &consumer_tag) {
            logger->INFO("Starting consumption of startup message.");
        })
        .onReceived([this](const AMQP::Message &message, uint64_t delivery_tag, bool redelivered) {
            logger->INFO("Recieved startup message");
            {
                std::lock_guard<std::mutex> lck(startup_mx);
            }
            setup_message = {message.body(), message.bodySize()};
            startup_cv.notify_one();
        })
        .onError([this](const char* message) {
            logger->INFO("Error in consumption of startup message.");
        });
    
    std::unique_lock<std::mutex> lck(startup_mx);
    startup_cv.wait(lck);



    /*
     * Once setup is done, start serving search requests
     *
     */

    channel->consume(REQUEST_CONN.QUEUE_NAME)
        .onSuccess([this](const std::string &consumer_tag) {
            logger->INFO("Consume operation started.");
        })
        .onReceived([this](const AMQP::Message &message, uint64_t delivery_tag, bool redelivered) {
            if (jobs_queue->size() < MAX_JOB_BACKLOG) {
                struct SearchJob job {message.body(), message.bodySize()};
                jobs_queue->push(job);
                channel->ack(delivery_tag);
                logger->INFO("JOB PUSHED.");
            } else {
                logger->WARN("JOB QUEUE FULL. REJECTING MESSAGE.");
            }
        })
        .onError([this](const char* message) {
            logger->INFO("Error.");
        });

    channel->consume(DATA_CONN.QUEUE_NAME)
        .onSuccess([](const std::string &consumer_tag) {

        });

    jobs_loop();
}

void RankerService::jobs_loop() {
    while (true) {
        if (jobs_queue->size() > 0) {
            struct SearchJob job = jobs_queue->back();

            string msg_str(job.data, job.data_size);
            json msg_data = json::parse(msg_str);
            json query_data = msg_data["tokens"];

            std::vector<char *> tokens;
            for (auto it = query_data.begin();it != query_data.end();it++) {
                string tmp = it.value().get<string>().c_str();
                tokens.push_back("HELLO");
            }

            std::vector<PageR> ranked_pages = ranker.rank(tokens, NUM_PAGES_TO_RETRIEVE);

            json results_data;
            results_data["id"] = query_data["id"];
            
            auto results = new std::vector<char *>(NUM_PAGES_TO_RETRIEVE);
            json temp(*results);
            results_data["results"] = temp;
            channel->publish(REQUEST_CONN.EXCHANGE_NAME, REQUEST_CONN.ROUTING_KEY, results_data.dump());

            jobs_queue->pop();
            logger->INFO("JOB COMPLETED.");
        }
    }
}