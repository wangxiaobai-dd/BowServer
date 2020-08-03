#include <assert.h>
#include "event_loop_thread.h"
#include "event_loop.h"
// #include "log.h"

void event_loop_thread::init(int i) 
{
    thread_name = "Thread-" + std::to_string(i+1);
}

void event_loop_thread::start() 
{
    loopThread = std::thread(event_loop_thread_run, shared_from_this());
    {
	std::unique_lock lock(mutex);
	cond.wait(lock, [this]{return eventLoop != nullptr; });
    }
    
    spdlog::info("event loop thread started, {}", thread_name);
}

void event_loop_thread::event_loop_thread_run(EventThreadPtr eventLoopThread) 
{
    std::unique_lock<std::mutex> lock(eventLoopThread->mutex);
    // 初始化event loop，之后通知主线程
    eventLoopThread->eventLoop = event_loop::event_loop_init(eventLoopThread->thread_name);
    spdlog::info("event loop thread init and signal, {}", eventLoopThread->thread_name);
    eventLoopThread->cond.notify_one();
    lock.unlock();

    //子线程event loop run
    eventLoopThread->eventLoop->event_loop_run();
}

