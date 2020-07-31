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
    printf("connection completed\n");
    return 0;
}

//数据读到buffer之后的callback
int onMessage(struct buffer *input, const TcpConnPtr& tcpConnection) {
    printf("get message from tcp connection %s\n", tcpConnection->name.c_str());
    printf("%s", input->data);

    struct buffer *output = buffer_new();
    int size = buffer_readable_size(input);
    for (int i = 0; i < size; i++) {
        buffer_append_char(output, rot13_char(buffer_read_char(input)));
    }
    tcp_connection_send_buffer(tcpConnection, output);
    return 0;
}

//数据通过buffer写完之后的callback
int onWriteCompleted(const TcpConnPtr& tcpConnection) {
    printf("write completed\n");
    return 0;
}

//连接关闭之后的callback
int onConnectionClosed(const TcpConnPtr& tcpConnection) {
    printf("connection closed\n");
    return 0;
}

int main(int argc, char** argv)
{
    //主线程event_loop
    EvLoopPtr eventLoop = event_loop::event_loop_init("main thread");

    //初始化acceptor
    AcceptorPtr acceptor = acceptor::acceptor_init(SERV_PORT);

    TcpServerPtr tcpServer = tcp_server::tcp_server_init(eventLoop, acceptor, onConnectionCompleted, onMessage, onWriteCompleted, onConnectionClosed, 4);

    tcpServer->start();

    eventLoop->event_loop_run("main thread");
}
