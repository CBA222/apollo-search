#include "data_service/mq/channel.h"
#include "data_service/mq/message.h"
#include "logging/stdout_logger.h"
#include "ranker/pageranker.h"
#include <nlohmann/json.hpp>
#include "ranker/tokenizer.h"

using json = nlohmann::json;

class RankerService
{
private:
    stdout_logger *logger;

    string socket_host;

    PageRanker *ranker;

    vector<Page> get_documents(std::vector<int> doc_ids);
public:

    RankerService();

    ~RankerService();

    void configure(
        string
    );

    void start();
};