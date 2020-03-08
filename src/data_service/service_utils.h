#include <cstdint>
#include <string>

using std::string;

enum MESSAGE_ID {
    GET_INVERTED_INDEX = 0,
    GET_DOCUMENT_DATA = 1,
    GET_SNIPPETS = 2
};

struct MQ_MESSAGE {
    const char* data;
    uint64_t data_size;
};

struct CONNINFO {
    string QUEUE_NAME;
    string EXCHANGE_NAME;
    string ROUTING_KEY;
};