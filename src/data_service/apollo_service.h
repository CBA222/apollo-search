
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifndef APOLLO_SERVICE_H
#define APOLLO_SERVICE_H

class ApolloService
{
public:
    virtual void start();
};

#endif