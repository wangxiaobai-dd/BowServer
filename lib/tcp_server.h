#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <csignal>

#include "acceptor.h"
#include "event_loop.h"
#include "thread_pool.h"
#include "buffer.h"
#include "tcp_connection.h"

struct tcp_server : public std::enable_shared_from_this<tcp_server>
{
    static TcpServerPtr instance()
    {
	if(!_instance)
	    _instance = std::shared_ptr<tcp_server>(new tcp_server());
	return _instance;
    }
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

    // 结束信号
    static void sigint_handler(int sig) { tcp_server::instance()->eventLoop->quit = true; }
    // 宕机信号
    static void sigsegv_handler(int sig) { std::signal(SIGSEGV, SIG_DFL);}

    static TcpServerPtr tcp_server_init(const EvLoopPtr& ,const AcceptorPtr&, const CallbackFunc & connectionCallBack, const MsgCallbackFunc& messageCallBack, const CallbackFunc& writeCompletedCallBack, const CallbackFunc& connectionClosedCallBack, int threadNum);

    static int handle_connection_established(const VarData& data);
private:
    tcp_server() = default;
    static TcpServerPtr _instance;
};



#endif
