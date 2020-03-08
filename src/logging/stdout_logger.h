#include "logging/defer_logger.h"
#include <iostream>

#ifndef STDOUT_LOGGER_H
#define STDOUT_LOGGER_H

class stdout_logger : public DeferLogger
{
protected:
    void process_message(string msg) override {
        std::cout << msg << std::endl;
    }
};

#endif