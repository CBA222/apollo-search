#include "data_service/mq/rr_broker.h"

int main(int argc, char* argv[]) {

    RRBroker *broker = new RRBroker(
        "tcp://127.0.0.1:1002",
        "tcp://127.0.0.1:1003"
    );

    broker->start();
}