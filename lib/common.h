//
// Created by shengym on 2019-07-07.
//

#ifndef YOLANDA_COMMON_H
#define YOLANDA_COMMON_H

#include <memory>
#include <functional>

#include    <sys/types.h>    /* basic system data types */
#include    <sys/socket.h>    /* basic socket definitions */
#include    <sys/time.h>    /* timeval{} for select() */
#include    <time.h>        /* timespec{} for pselect() */
#include    <netinet/in.h>    /* sockaddr_in{} and other Internet defns */
#include    <arpa/inet.h>    /* inet(3) functions */
#include    <errno.h>
#include    <fcntl.h>        /* for nonblocking */
#include    <netdb.h>
#include    <signal.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/stat.h>    /* for S_xxx file mode constants */
#include    <sys/uio.h>        /* for iovec{} and readv/writev */
#include    <unistd.h>
#include    <sys/wait.h>
#include    <sys/un.h>        /* for Unix domain sockets */

#include    <sys/select.h>    /* for convenience */
#include    <sys/sysctl.h>
#include    <poll.h>        /* for convenience */
#include    <strings.h>        /* for convenience */
#include    <sys/ioctl.h>
#include    <pthread.h>

#include    <sys/epoll.h>

void err_dump(const char *, ...);

void err_msg(const char *, ...);

void err_quit(const char *, ...);

void err_ret(const char *, ...);

void err_sys(const char *, ...);

void error(int status, int err, char *fmt, ...);

char *sock_ntop(const struct sockaddr_in *sin, socklen_t salen);

size_t readn(int fd, void *vptr, size_t n);

size_t read_message(int fd, char *buffer, size_t length);

size_t readline(int fd, char *buffer, size_t length);

// int tcp_server(int port);

int tcp_server_listen(int port);

int tcp_nonblocking_server_listen(int port);

void make_nonblocking(int fd);

int tcp_client(char *address, int port);

#define    SERV_PORT      43211
#define    MAXLINE        4096
#define    UNIXSTR_PATH   "/var/lib/unixstream.sock"
#define    LISTENQ        1024
#define    BUFFER_SIZE    4096

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

typedef std::function<int(const TcpConnPtr&)> CallbackFunc;
typedef std::function<int(struct buffer*, const TcpConnPtr&)> MsgCallbackFunc;

#define LISTEN_SIZE 128
#define SOCKET_BUF_SIZE 128 * 1024

enum DataType
{
    NONE_TYPE = 0,
    EVLOOP_TYPE = 1,
    SERVER_TYPE = 2,
    CONN_TYPE = 3
};

#endif //YOLANDA_COMMON_H
