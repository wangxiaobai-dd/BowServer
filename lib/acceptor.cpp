#include <assert.h>
#include "acceptor.h"

AcceptorPtr acceptor::acceptor_init(int port) 
{
    auto acceptorPtr = std::make_shared<acceptor>();
    acceptorPtr->listen_port = port;
    acceptorPtr->listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 设置非阻塞
    make_nonblocking(acceptorPtr->listen_fd);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    socklen_t size = SOCKET_BUF_SIZE;
    setsockopt(acceptorPtr->listen_fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    setsockopt(acceptorPtr->listen_fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
    int on = 1;
    setsockopt(acceptorPtr->listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int rt1 = bind(acceptorPtr->listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (rt1 < 0) {
        error(1, errno, "bind failed ");
    }

    int rt2 = listen(acceptorPtr->listen_fd, LISTENQ);
    if (rt2 < 0) {
        error(1, errno, "listen failed ");
    }
    
    return acceptorPtr;
}
