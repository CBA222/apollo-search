#include "data_service/mq/deferred_object.h"

class DeferredConsumer : public DeferredObject
{
private:
public:
    DeferredConsumer& onReceived(void (*func)(int));
};