#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "acceptor.h"
#include "event_loop.h"
#include "thread_pool.h"
#include "buffer.h"
#include "tcp_connection.h"

// typedef std::function<int(TcpConnPtr)> CallbackFunc
// typedef std::function<int(struct buffer*, TcpConnPtr)> MsgCallbackFunc

struct tcp_server : public std::enable_shared_from_this<tcp_server>
{
    int port;
    EvLoopPtr eventLoop;
    AcceptorPtr acceptor;

    CallbackFunc connectionCompletedCallBack;
    MsgCallbackFunc messageCallBack;
    CallbackFunc writeCompletedCallBack;
    CallbackFunc connectionClosedCallBack;
    int threadNum;
    ThreadPoolPtr threadPool;

    void start();

    static TcpServerPtr tcp_server_init(const EvLoopPtr& ,const AcceptorPtr&, const CallbackFunc & connectionCallBack, const MsgCallbackFunc& messageCallBack, const CallbackFunc& writeCompletedCallBack, const CallbackFunc& connectionClosedCallBack, int threadNum);
};

int handle_connection_established(const VarData& data);


#endif
