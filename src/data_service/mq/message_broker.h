#include <zmq.h>

class MessageBroker
{
private:

public:
    MessageBroker();
    ~MessageBroker();

    void add_queue();
    void add_exchange();
    void bind_exchange_queue();

    void start();
};