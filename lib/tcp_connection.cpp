#include "tcp_connection.h"
#include "utils.h"
#include "log.h"

// 建立新连接
TcpConnPtr tcp_connection::tcp_connection_new(int connectedFd, const EvLoopPtr& eventLoop, const CallbackFunc& connectionCompletedCallBack, const CallbackFunc& connectionClosedCallBack, const MsgCallbackFunc& messageCallBack, const CallbackFunc& writeCompletedCallBack)
{
    auto tcpConnection = std::make_shared<tcp_connection>();
    tcpConnection->writeCompletedCallBack = writeCompletedCallBack;
    tcpConnection->messageCallBack = messageCallBack;
    tcpConnection->connectionCompletedCallBack = connectionCompletedCallBack;
    tcpConnection->connectionClosedCallBack = connectionClosedCallBack;
    tcpConnection->eventLoop = eventLoop;
    tcpConnection->input_buffer = buffer::buffer_new();
    tcpConnection->output_buffer = buffer::buffer_new();

    tcpConnection->name = "connection-" + std::to_string(connectedFd);

    // 事件
    auto channel1 = channel::channel_new(connectedFd, EVENT_READ, handle_read, handle_write, tcpConnection, CONN_TYPE);
    tcpConnection->channel = channel1;

    // 连接完成回调
    if (tcpConnection->connectionCompletedCallBack != NULL) 
        tcpConnection->connectionCompletedCallBack(tcpConnection);

    event_loop::event_loop_add_channel_event(tcpConnection->eventLoop, connectedFd, tcpConnection->channel);
    return tcpConnection;
}


// 连接关闭
int tcp_connection::handle_connection_closed(const TcpConnPtr& tcpConnection) 
{
    auto eventLoop = tcpConnection->eventLoop;
    auto channel = tcpConnection->channel;

    event_loop::event_loop_remove_channel_event(eventLoop, channel->fd, channel);

    // 回调
    if (tcpConnection->connectionClosedCallBack) 
        tcpConnection->connectionClosedCallBack(tcpConnection);
}

// 读事件回调
int tcp_connection::handle_read(const VarData& data)
{
    auto tcpConnection = std::get<TcpConnPtr>(data);
    struct buffer* input_buffer = tcpConnection->input_buffer;
    auto channel = tcpConnection->channel;

    if (input_buffer->buffer_socket_read(channel->fd) > 0) 
    {
        //应用程序真正读取Buffer里的数据
	if (tcpConnection->messageCallBack)
	    tcpConnection->messageCallBack(input_buffer, tcpConnection);
    } 
    else 
        handle_connection_closed(tcpConnection);
}

// 发送缓冲区可以往外写
// 把channel对应的output_buffer不断往外发送
int tcp_connection::handle_write(const VarData& data) 
{
    auto tcpConnection = std::get<TcpConnPtr>(data);
    auto eventLoop = tcpConnection->eventLoop;
    assertInSameThread(eventLoop);

    struct buffer* output_buffer = tcpConnection->output_buffer;
    auto channel = tcpConnection->channel;

    ssize_t nwrited = write(channel->fd, output_buffer->data + output_buffer->readIndex,
                            output_buffer->buffer_readable_size());
    if (nwrited > 0) 
    {
        //已读nwrited字节
        output_buffer->readIndex += nwrited;
        //如果数据完全发送出去，就不需要继续了
        if (output_buffer->buffer_readable_size() == 0)
	    channel->channel_write_event_disable();
        
        //回调writeCompletedCallBack
        if (tcpConnection->writeCompletedCallBack)
            tcpConnection->writeCompletedCallBack(tcpConnection);
    } 
    else 
        yolanda_msgx("handle_write for tcp connection %s", tcpConnection->name.c_str());
}

int tcp_connection::tcp_connection_send_buffer(struct buffer* buffer) 
{
    int size = buffer->buffer_readable_size();
    int result = tcp_connection_send_data(buffer->data + buffer->readIndex, size);
    buffer->readIndex += size;
    return result;
}

int tcp_connection::tcp_connection_send_data(void* data, int size) 
{
    size_t nwrited = 0;
    size_t nleft = size;
    int fault = 0;

    // 先往套接字尝试发送数据
    if (!channel->channel_write_event_is_enabled() && output_buffer->buffer_readable_size() == 0)
    {
	nwrited = write(channel->fd, data, size);
	if (nwrited >= 0)
	    nleft = nleft - nwrited;
	else 
	{
	    nwrited = 0;
	    if (errno != EWOULDBLOCK) 
	    {
		if (errno == EPIPE || errno == ECONNRESET) 
		    fault = 1;
	    }
	    // EAGIN EWOULDBLOCK
	}
    }
    if (!fault && nleft > 0) 
    {
	// 拷贝到Buffer中，Buffer的数据由框架接管
	output_buffer->buffer_append(static_cast<char*>(data) + nwrited, nleft);
	if (!channel->channel_write_event_is_enabled())
	    channel->channel_write_event_enable();
    }
    return nwrited;
}

void tcp_connection::tcp_connection_shutdown() 
{
    if (shutdown(channel->fd, SHUT_WR) < 0)
	yolanda_msgx("tcp_connection_shutdown failed, socket == %d", channel->fd);
}
