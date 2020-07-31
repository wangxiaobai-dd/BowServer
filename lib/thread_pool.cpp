#include <assert.h>
#include "utils.h"
#include "thread_pool.h"

// 创建线程池
ThreadPoolPtr thread_pool::thread_pool_new(const EvLoopPtr& mainLoop, int threadNumber)
{
    auto threadPool = std::make_shared<thread_pool>();
    threadPool->mainLoop = mainLoop;
    threadPool->thread_number = threadNumber;
    return threadPool;
}

// 启动线程池 main thread发起
void thread_pool::thread_pool_start(ThreadPoolPtr& threadPool) 
{
    assert(!threadPool->started);
    assertInSameThread(threadPool->mainLoop);

    threadPool->started = 1;

    if (threadPool->thread_number <= 0)
        return;

    for (int i = 0; i < threadPool->thread_number; ++i) 
    {
	auto threadPtr = std::make_shared<event_loop_thread>();	
	event_loop_thread::event_loop_thread_init(threadPtr, i);
	event_loop_thread::event_loop_thread_start(threadPtr);
	threadPool->eventLoopThreadVec.push_back(threadPtr);
    }
}

// 一定是main thread中选择
EvLoopPtr thread_pool::thread_pool_get_loop()
{
    assert(started);
    assertInSameThread(mainLoop);

    // 优先选择当前主线程
    auto selected = mainLoop;

    // 从线程池中按照顺序挑选出一个线程 选择连接最少的线程
    for (int i = 0; i < thread_number; ++i)
    {
	if(selected->get_event_size() > eventLoopThreadVec[i]->eventLoop->get_event_size())
	    selected = eventLoopThreadVec[i]->eventLoop;
    }
    return selected;
}
