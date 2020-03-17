#include "data_service/mq/rr_broker.h"
#include <toml11/toml.hpp>

/*
 * Configure and initalize a RRBroker from a toml configuration file.
 * toml file should be in this format:
 *  -frontend_host: ''
 *  -backend_host: ''
 */
RRBroker* init_rrbroker_toml(string filepath) {
    auto data = toml::parse(filepath);
    string fe_host = toml::find<string>(data, "frontend_host");
    string be_host = toml::find<string>(data, "backend_host");

    return new RRBroker(fe_host, be_host);
};