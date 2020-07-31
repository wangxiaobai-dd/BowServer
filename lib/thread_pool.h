#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "event_loop.h"
#include "event_loop_thread.h"

struct thread_pool 
{
    // 创建thread_pool的主线程
    EvLoopPtr mainLoop;
    // 是否已经启动
    int started = 0;
    // 线程数目
    int thread_number = 0;

    std::vector<EventThreadPtr> eventLoopThreadVec;

    EvLoopPtr thread_pool_get_loop();
    
    static ThreadPoolPtr thread_pool_new(const EvLoopPtr& mainLoop, int threadNumber);
    static void thread_pool_start(ThreadPoolPtr&);
};

#endif
