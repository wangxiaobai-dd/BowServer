#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <map>
#include <list>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <thread>

#include <pthread.h>
#include "channel.h"
#include "event_dispatcher.h"
#include "common.h"

enum 
{
    CHANNEL_ADD = 1,
    CHANNEL_REMOVE = 2,
    CHANNEL_UPDATE = 3,
};

/*
 * 事件循环
 */
struct event_loop : public std::enable_shared_from_this<event_loop> 
{
    int quit = 0;
    int is_handle_pending = 0;
    std::string threadName;

    std::thread::id ownThreadID;
    std::mutex mutex;
    std::condition_variable cond;

    // 事件分发
    EvDispatchPtr evDispatchPtr;

    void init(const std::string& _threadName);
    void event_loop_run();
    int get_event_size();
    
    static EvLoopPtr event_loop_init(const std::string& _threadName);
    
    static void event_loop_add_channel_event(EvLoopPtr&, int fd, const ChanPtr&);
    static void event_loop_remove_channel_event(EvLoopPtr&, int fd, const ChanPtr&);
    static void event_loop_update_channel_event(EvLoopPtr&, int fd, const ChanPtr&);
    static int event_loop_handle_pending_add(EvLoopPtr&, int fd, ChanPtr&);
    static int event_loop_handle_pending_remove(EvLoopPtr&, int fd, ChanPtr&);
    static int event_loop_handle_pending_update(EvLoopPtr&, int fd, ChanPtr&);
    
    // dispather派发完事件之后，调用该方法通知event_loop执行对应事件的相关callback方法
    // res: EVENT_READ | EVENT_READ等
    static int channel_event_activate(EvLoopPtr&, int fd, int res);
};



#endif
