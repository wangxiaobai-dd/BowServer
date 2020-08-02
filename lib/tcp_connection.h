#ifndef TCP_CONNECTION
#define TCP_CONNECTION

#include "event_loop.h"
#include "channel.h"
#include "buffer.h"
#include "tcp_server.h"

struct tcp_connection 
{
    tcp_connection(){};
    ~tcp_connection()
    {
	buffer::buffer_free(input_buffer);		
	buffer::buffer_free(output_buffer);
    };

    EvLoopPtr eventLoop;
    ChanPtr channel;
    std::string name;
    struct buffer* input_buffer;   // 接收缓冲区
    struct buffer* output_buffer;  // 发送缓冲区

    CallbackFunc connectionCompletedCallBack;
    MsgCallbackFunc messageCallBack;
    CallbackFunc writeCompletedCallBack;
    CallbackFunc connectionClosedCallBack;

    // 应用层调用入口
    int tcp_connection_send_data(void* data, int size);
    int tcp_connection_send_buffer(struct buffer* buffer);
    void tcp_connection_shutdown();

    static TcpConnPtr tcp_connection_new(int connectedFd, const EvLoopPtr& eventLoop, const CallbackFunc&connectionCompletedCallBack, const CallbackFunc& connectionClosedCallBack, const MsgCallbackFunc& messageCallBack, const CallbackFunc& writeCompleteCallBack);

    static int handle_read(const VarData& data);
    static int handle_write(const VarData& data);
    static int handle_connection_closed(const TcpConnPtr& tcpConnection);
};


// int tcp_connection_append_buffer(struct tcp_connection *tcpConnection);
#endif
