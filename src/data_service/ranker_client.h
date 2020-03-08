#include <vector>
#include "apollo_service.h"

using std::vector;

class RankerClient
{
private:
    //AMQP::TcpChannel *channel;
    stdout_logger *logger;

public:
    vector<float> get_vector_space(int doc_id);
};