#include "acceptor.h"
#include "common.h"
#include "event_loop.h"
#include "tcp_server.h"

char rot13_char(char c) {
    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    else
        return c;
}

//连接建立之后的callback
int onConnectionCompleted(const TcpConnPtr& tcpConnection) {
    spdlog::info("connection completed");
    return 0;
}

// 数据读到buffer之后的callback
int onMessage(struct buffer* input, const TcpConnPtr& tcpConnection) {
    spdlog::info("get message get message from tcp connection:{}", tcpConnection->name);
    spdlog::info("data:{}", input->data + input->readIndex);

    struct buffer* output = buffer::buffer_new();
    int size = input->buffer_readable_size();
    for (int i = 0; i < size; i++) 
	output->buffer_append_char(rot13_char(input->buffer_read_char()));

    tcpConnection->tcp_connection_send_buffer(output);
    buffer::buffer_free(output);
    return 0;
}

//数据通过buffer写完之后的callback
int onWriteCompleted(const TcpConnPtr& tcpConnection) {
    spdlog::info("write completed");
    return 0;
}

//连接关闭之后的callback
int onConnectionClosed(const TcpConnPtr& tcpConnection) {
    spdlog::info("connection cloesd, name:{}", tcpConnection->name);
    return 0;
}

int main(int argc, char** argv)
{
    // 主线程event_loop
    EvLoopPtr eventLoop = event_loop::event_loop_init("main thread");
    // 初始化acceptor
    AcceptorPtr acceptor = acceptor::acceptor_init(SERV_PORT);

    TcpServerPtr tcpServer = tcp_server::tcp_server_init(eventLoop, acceptor, onConnectionCompleted, onMessage, onWriteCompleted, onConnectionClosed, 1);

    tcpServer->start();
    eventLoop->event_loop_run();
}
