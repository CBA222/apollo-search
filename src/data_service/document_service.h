#include <amqpcpp.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <queue>
#include "data_service/apollo_service.h"

enum MESSAGE_ID {
    GET_INVERTED_INDEX = 0,
    GET_DOCUMENT_DATA = 1,
    GET_SNIPPETS = 2
}

class SearchDataService : public ApolloService
{
public:
    void start();
}