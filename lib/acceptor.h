#ifndef ACCEPTOR_H
#define ACCEPTOR_H

// #include "base.h"
#include "common.h"

//using AcceptorPtr = std::shared_ptr<acceptor>;
struct acceptor
{
    int listen_port;
    int listen_fd;
    static AcceptorPtr acceptor_init(int port);
};



#endif
