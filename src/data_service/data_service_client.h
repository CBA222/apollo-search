enum MESSAGE_TYPE {
    SETUP_TOKENS,
    SETUP_INDEX,
    SETUP_MAPPINGS
}

class DataServiceClient
{
public:
    virtual void send_message(MESSAGE_TYPE msg_type);
}