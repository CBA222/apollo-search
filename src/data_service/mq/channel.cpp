#include "data_service/mq/channel.h"

using namespace MQ;

Channel::Channel(string addr) {
    address = addr;
    logger = new stdout_logger();
}

Channel::~Channel() {
    if (consuming) {
        consume_thread_.join();
    }
}

Message Channel::publish_blocking(string s) {
    zmq::message_t msg;
    context = zmq::context_t(1);
    socket = zmq::socket_t(context, ZMQ_REP);

    socket.send(zmq::buffer(s));
    socket.recv(msg);

    Message message {(char*)msg.data(), (int)msg.size()};

    socket.close();
    context.close();

    return message;
}

Channel& Channel::publish(string s) {
    socket.send(zmq::buffer(s));
}

void Channel::consume_loop() {
    context = zmq::context_t(1);
    socket = zmq::socket_t(context, ZMQ_REP);
    controller = zmq::socket_t(context, ZMQ_REP);

    logger->INFO("Channel bound to " + address);

    socket.bind(address);

    zmq::pollitem_t items [] = {
        {static_cast<void*>(socket), 0, ZMQ_POLLIN, 0},
        {static_cast<void*>(controller), 0, ZMQ_POLLIN, 0}
    };


    while (1) {
        zmq::message_t message;
        int more;

        zmq::poll(&items[0], 2, 10);

        if (items[0].revents & ZMQ_POLLIN) {
            while (1) {
                socket.recv(message);
                size_t more_size = sizeof(more);
                socket.getsockopt(ZMQ_RCVMORE, &more, &more_size);
                
                if (!more) {
                    break;
                }
            }
            Message msg {(char *)message.data(), (int)message.size()};
            consume_callback(msg);
        }

        if (items[1].revents & ZMQ_POLLIN) {
            break;
        }

        if (close_thread == true) {
            break;
        }
    }

    logger->INFO("Shutting down channel.");

    socket.setsockopt(ZMQ_LINGER, 0);
    controller.setsockopt(ZMQ_LINGER, 0);
    socket.close();
    controller.close();
    context.close();
}

Channel& Channel::consume() {
    consuming = true;
    consume_thread_ = std::thread(&Channel::consume_loop, this);
    logger->INFO("started thread");
    return *this;
}

Channel& Channel::onReceived(std::function<void(Message)> func) {
    logger->INFO("Callback set");
    consume_callback = func;
}

void Channel::close() {
    close_thread = true;
}