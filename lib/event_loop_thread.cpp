#include <assert.h>
#include "event_loop_thread.h"
#include "event_loop.h"
#include "log.h"

void event_loop_thread::event_loop_thread_init(EventThreadPtr& eventLoopThread, int i) 
{
    char buf[20] = {0};
    sprintf(buf, "Thread-%d\0", i + 1);
    eventLoopThread->thread_name = buf;
}

void event_loop_thread::event_loop_thread_start(EventThreadPtr& eventLoopThread) 
{
    eventLoopThread->loopThread = std::thread(event_loop_thread_run, eventLoopThread);
    {
	std::unique_lock lock(eventLoopThread->mutex);
	eventLoopThread->cond.wait(lock, [&eventLoopThread]{return eventLoopThread->eventLoop != nullptr; });
    }
    yolanda_msgx("event loop thread started, %s", eventLoopThread->thread_name.c_str());
}

void event_loop_thread::event_loop_thread_run(EventThreadPtr eventLoopThread) 
{
    std::unique_lock<std::mutex> lock(eventLoopThread->mutex);
    // 初始化event loop，之后通知主线程
    eventLoopThread->eventLoop = event_loop::event_loop_init(eventLoopThread->thread_name);
    yolanda_msgx("event loop thread init and signal, %s", eventLoopThread->thread_name.c_str());
    eventLoopThread->cond.notify_one();
    lock.unlock();

    //子线程event loop run
    eventLoopThread->eventLoop->event_loop_run(eventLoopThread->thread_name);
}

