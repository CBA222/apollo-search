#include <string>
#include <map>
#include <queue>
#include <thread>
#include "logging/logger.h"

using std::string;

#ifndef DEFER_LOGGER_H
#define DEFER_LOGGER_H

class DeferLogger : public Logger
{
private:
    std::thread thread_;
    std::map<LOG_LEVEL, std::queue<string>> backlog;

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