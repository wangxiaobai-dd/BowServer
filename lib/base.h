#include <memory>

struct event_loop; 
struct channel;
struct channel_element;
struct event_dispatcher;
struct tcp_connection;
struct acceptor;
struct tcp_server;
struct thread_pool;
struct event_loop_thread;

typedef std::shared_ptr<event_loop> EvLoopPtr;
typedef std::shared_ptr<channel> ChanPtr;
typedef std::shared_ptr<channel_element> ChanElePtr;
typedef std::shared_ptr<event_dispatcher> EvDispatchPtr;
typedef std::shared_ptr<tcp_connection> TcpConnPtr;
typedef std::shared_ptr<acceptor> AcceptorPtr;
typedef std::shared_ptr<tcp_server> TcpServerPtr;
typedef std::shared_ptr<thread_pool> ThreadPoolPtr;
typedef std::shared_ptr<event_loop_thread> EventThreadPtr;
