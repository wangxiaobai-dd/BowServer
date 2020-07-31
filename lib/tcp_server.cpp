#include <assert.h>
#include "common.h"
#include "tcp_server.h"
#include "thread_pool.h"
#include "utils.h"
#include "log.h"

TcpServerPtr tcp_server::tcp_server_init(const EvLoopPtr& eventLoop, const AcceptorPtr& acceptor, const CallbackFunc& connectionCompletedCallBack, const MsgCallbackFunc& messageCallBack, const CallbackFunc& writeCompletedCallBack, const CallbackFunc& connectionClosedCallBack, int threadNum) {
    auto tcpServer = std::make_shared<tcp_server>();
    tcpServer->eventLoop = eventLoop;
    tcpServer->acceptor = acceptor;
    tcpServer->connectionCompletedCallBack = connectionCompletedCallBack;
    tcpServer->messageCallBack = messageCallBack;
    tcpServer->writeCompletedCallBack = writeCompletedCallBack;
    tcpServer->connectionClosedCallBack = connectionClosedCallBack;
    tcpServer->threadNum = threadNum;
    tcpServer->threadPool = thread_pool::thread_pool_new(eventLoop, threadNum);
    return tcpServer;
}

void tcp_server::start()
{
    // 开启多个线程
    thread_pool::thread_pool_start(this->threadPool);

    // acceptor主线程， 同时把tcpServer作为参数传给channel对象
    // 主线程回调参数为tcpServer
    auto channel = channel::channel_new(acceptor->listen_fd, EVENT_READ, handle_connection_established, nullptr, shared_from_this(), SERVER_TYPE);
    // 主循环增加监听
    event_loop::event_loop_add_channel_event(this->eventLoop, channel->fd, channel);
}

void make_nonblocking(int fd) 
{
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

int handle_connection_established(const VarData& data) 
{
    auto tcpServer = std::get<TcpServerPtr>(data); 
    auto acceptor = tcpServer->acceptor;
    int listenfd = acceptor->listen_fd;

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int connected_fd = accept(listenfd, (struct sockaddr *) &client_addr, &client_len);
    make_nonblocking(connected_fd);

    yolanda_msgx("new connection established, socket == %d", connected_fd);

    // choose event loop from the thread pool
    auto eventLoop = tcpServer->threadPool->thread_pool_get_loop();

    // create a new tcp connection
    auto tcpConnection = tcp_connection::tcp_connection_new(connected_fd, eventLoop, tcpServer->connectionCompletedCallBack, tcpServer->connectionClosedCallBack, tcpServer->messageCallBack, tcpServer->writeCompletedCallBack);

    return 0;
}
