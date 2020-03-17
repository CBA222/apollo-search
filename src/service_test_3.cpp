#include "data_service/document_service.h"
#include <iostream>
#include <toml11/toml.hpp>

using std::string;

string load_config(string filepath, string *buf) {
    const std::string fname(filepath);
    const toml::value data = toml::parse(fname);

    const auto& data_db = toml::find(data, "postgres");

    buf[0] = toml::find<string>(data_db, "host");
    buf[1] = toml::find<string>(data_db, "port");
    buf[2] = toml::find<string>(data_db, "user");
    buf[3] = toml::find<string>(data_db, "password");
    buf[4] = toml::find<string>(data_db, "dbname");

    string final = "host=" + buf[0] + " port=" + buf[1] + " user=" + buf[2] + " password=" + buf[3] + " dbname=" + buf[4];
    return final;
}

int main(int argc, char* argv[]) {

    string param_vals[5];
    string sql_string = load_config("config.toml", param_vals);

    SearchDataService *service = new SearchDataService();
    service->configure("tcp://127.0.0.1:7777", sql_string);
    std::cout <<"bfore start" << std::endl;
    service->start();

    //MQ::Channel *channel = new MQ::Channel("tcp://127.0.0.1:1003");

    

    /*
    channel->onReceived([&channel](MQ::Message msg) {
        std::cout << msg.data << std::endl;
        channel->publish("eminem");
    });

    channel->consume();
    while (1) {

    }
    */
}