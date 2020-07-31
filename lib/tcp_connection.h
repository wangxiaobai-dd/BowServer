#ifndef TCP_CONNECTION
#define TCP_CONNECTION

#include "event_loop.h"
#include "channel.h"
#include "buffer.h"
#include "tcp_server.h"

struct tcp_connection 
{
    EvLoopPtr eventLoop;
    ChanPtr channel;
    std::string name;
    struct buffer* input_buffer;   //接收缓冲区
    struct buffer* output_buffer;  //发送缓冲区

    CallbackFunc connectionCompletedCallBack;
    MsgCallbackFunc messageCallBack;
    CallbackFunc writeCompletedCallBack;
    CallbackFunc connectionClosedCallBack;

    void * data; //for callback use: http_server
    void * request; // for callback use
    void * response; // for callback use

    static TcpConnPtr tcp_connection_new(int connectedFd, const EvLoopPtr& eventLoop, const CallbackFunc&connectionCompletedCallBack, const CallbackFunc& connectionClosedCallBack, const MsgCallbackFunc& messageCallBack, const CallbackFunc& writeCompleteCallBack);
};

int handle_read(const VarData& data);
int handle_write(const VarData& data);

// 应用层调用入口
int tcp_connection_send_data(const TcpConnPtr& tcpConnection, void *data, int size);
// 应用层调用入口
int tcp_connection_send_buffer(const TcpConnPtr& tcpConnection, struct buffer * buffer);

void tcp_connection_shutdown(const TcpConnPtr& tcpConnection);
// int tcp_connection_append_buffer(struct tcp_connection *tcpConnection);
#endif
