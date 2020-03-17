#include "data_service/mq/rr_broker.h"
//#include <czmq.h>

static int s_interrupted = 0;
static void s_signal_handler(int sv) {
    s_interrupted = 1;
}

static void s_catch_signals() {
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction (SIGINT, &action, NULL);
    sigaction (SIGTERM, &action, NULL);
}

RRBroker::RRBroker(string fe_host, string be_host) {
    frontend_host = fe_host;
    backend_host = be_host;
}

void RRBroker::start() {

    logger = new stdout_logger();

    zmq::context_t context(1);
    zmq::socket_t frontend(context, ZMQ_REP);
    zmq::socket_t backend(context, ZMQ_REQ);

    frontend.bind(frontend_host);
    backend.bind(backend_host);

    logger->INFO("RRBroker bound to frontend at " + frontend_host);
    logger->INFO("RRBroker bound to backend at " + backend_host);

    s_catch_signals();

    zmq::pollitem_t items [] = {
        {static_cast<void*>(frontend), 0, ZMQ_POLLIN, 0},
        {static_cast<void*>(backend), 0, ZMQ_POLLIN, 0}
    };

    while (1) {
        zmq::message_t message;
        int more;

        zmq::poll (&items[0], 2, -1);

        if (items [0].revents & ZMQ_POLLIN) {
            while (1) {
                frontend.recv(message);
                logger->INFO("Message recieved on frontend.");
                size_t more_size = sizeof(more);
                frontend.getsockopt(ZMQ_RCVMORE, &more, &more_size);
                backend.send(message, more ? ZMQ_MORE : 0);
                
                if (!more) {
                    break;
                }
            }
        }

        if (items [1].revents & ZMQ_POLLIN) {
            while (1) {
                backend.recv(message);
                logger->INFO("Message recieved on backend.");
                size_t more_size = sizeof(more);
                backend.getsockopt(ZMQ_RCVMORE, &more, &more_size);
                frontend.send(message, more ? ZMQ_MORE : 0);

                if (!more) {
                    break;
                }
            }
        }

        if (s_interrupted == 1) {
            logger->INFO("Interrupt Recieved. Processing...");
            break;
        }
    }

    frontend.setsockopt(ZMQ_LINGER, 0);
    backend.setsockopt(ZMQ_LINGER, 0);
    

    frontend.close();
    backend.close();
    context.close();

    /*
    zmq::socket_t test_socket(context, ZMQ_REP);
    test_socket.bind("tcp://*:1001");

    s_catch_signals();

    while (true) {
        zmq::message_t msg;
        
        try {
            auto ret = test_socket.recv(msg);
        }
        catch (zmq::error_t& e) {
            std::cout << "Recieved Interrupt. Processing..." << std::endl;
        }
        //auto ret = test_socket.recv(msg);

        if (s_interrupted == 1) {
            test_socket.close();
            context.close();
            break;
        }
        
        std::cout << "World!" << (char *)(msg.data()) << std:: endl;
        std::cout << msg.size() << std::endl;
        test_socket.send(zmq::str_buffer("World!"));
    }

    */

    /*

    zmq::context_t context(1);
    zmq::socket_t frontend(context, ZMQ_ROUTER);
    zmq::socket_t backend(context, ZMQ_ROUTER);

    frontend.bind("ipc://frontend.ipc");
    backend.bind("ipd://backend.ipc");

    zmq::proxy(frontend, backend);

    frontend.send(zmq::str_buffer("Hello"));


    while (true) {
        zmq::pollitem_t items[] = {
            { backend, 0, ZMQ_POLLIN, 0},
            { frontend, 0, ZMQ_POLLIN, 0}
        };

        if (worker_queue.size() > 0) {
            zmq::poll(&items[0], 2, -1);
        } else {
            zmq::poll(&items[1], 1, -1);
        }

        if (items[0].revents & ZMQ_POLLIN) {
            worker_queue.push(s_recv(backend));
        }
    }
    */
}