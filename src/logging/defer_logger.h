#include <string>
#include <map>
#include <queue>
#include <thread>
#include <mutex>
#include <unordered_map>
#include "logging/logger.h"

using std::string;

#ifndef DEFER_LOGGER_H
#define DEFER_LOGGER_H

class DeferLogger : public Logger
{
private:
    std::thread thread_;
    std::map<LOG_LEVEL, std::queue<string>> backlog;
    std::mutex queue_mutex;

    std::unordered_map<LOG_LEVEL, std::string> LEVEL_NAMES {
        { LOG_LEVEL::INFO, "INFO" },
        { LOG_LEVEL::WARN, "WARN" },
        { LOG_LEVEL::ERROR, "ERROR" },
        { LOG_LEVEL::DEBUG, "DEBUG" },
        { LOG_LEVEL::CRITICAL, "CRITICAL" }
    };

    void LOG(string, LOG_LEVEL);
    void message_loop();
protected:
    virtual void process_message(string) {};
public:
    DeferLogger();
    ~DeferLogger();
    
    void INFO(string) override;
    void WARN(string) override;
    void ERROR(string) override;
    void DEBUG(string) override;
    void CRITICAL(string) override;

    void set_level() override;
};

#endif