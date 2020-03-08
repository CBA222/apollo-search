#include <iostream>
#include "logging/stdout_logger.h"
#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>
#include <string>

#ifndef APOLLO_SERVICE_H
#define APOLLO_SERVICE_H



class ApolloService
{
    
public:
    virtual void start() {};
};

#endif