#include <string>
#include <iostream>
#include "log_level.h"

using std::string;

#ifndef LOGGER_H
#define LOGGER_H

class Logger
{
public:
    virtual void INFO(string) {};
    virtual void WARN(string) {};
    virtual void ERROR(string) {};
    virtual void DEBUG(string) {};
    virtual void CRITICAL(string) {};

    virtual void set_level() {};
};

#endif