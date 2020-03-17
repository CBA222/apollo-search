#include "defer_logger.h"

DeferLogger::DeferLogger() {
    backlog[LOG_LEVEL::INFO] = std::queue<string>();
    backlog[LOG_LEVEL::DEBUG] = std::queue<string>();
    backlog[LOG_LEVEL::WARN] = std::queue<string>();
    backlog[LOG_LEVEL::ERROR] = std::queue<string>();
    backlog[LOG_LEVEL::CRITICAL] = std::queue<string>();

    thread_ = std::thread(&DeferLogger::message_loop, this);
}

DeferLogger::~DeferLogger() {
    thread_.join();
}

void DeferLogger::LOG(string msg, LOG_LEVEL level) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    backlog[level].push(msg);
}

void DeferLogger::message_loop() {
    while (true) {
        for (auto it = backlog.begin();it != backlog.end();it++) {
            if (!((it->second).empty())) {
                string msg = (it->second).back();
                msg = "[" + LEVEL_NAMES[it->first] + "] " + msg;
                process_message(msg);
                (it->second).pop();
            }
            
        }
    }
}

void DeferLogger::set_level() {

}

void DeferLogger::INFO(string msg) {
    LOG(msg, LOG_LEVEL::INFO);
}

void DeferLogger::DEBUG(string msg) {
    LOG(msg, LOG_LEVEL::DEBUG);
}

void DeferLogger::WARN(string msg) {
    LOG(msg, LOG_LEVEL::WARN);
}

void DeferLogger::ERROR(string msg) {
    LOG(msg, LOG_LEVEL::ERROR);
}

void DeferLogger::CRITICAL(string msg) {
    LOG(msg, LOG_LEVEL::CRITICAL);
}