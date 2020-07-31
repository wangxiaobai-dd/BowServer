#ifndef EVENT_LOOP_THREAD_H
#define EVENT_LOOP_THREAD_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "common.h"

struct event_loop_thread 
{
    // 子线程循环
    EvLoopPtr eventLoop;

    std::mutex mutex;
    std::condition_variable cond;
    std::string thread_name;
    int thread_count = 0;

    std::thread loopThread;
    // 初始化已经分配内存的event_loop_thread
    static void event_loop_thread_init(EventThreadPtr&, int);
    // 由主线程调用，初始化一个子线程，并且让子线程开始运行event_loop
    static void event_loop_thread_start(EventThreadPtr&);
    static void event_loop_thread_run(EventThreadPtr eventLoopThread);
};


#endif
