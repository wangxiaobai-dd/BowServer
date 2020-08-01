#include <iostream>
#include <assert.h>
#include "event_loop.h"
#include "common.h"
#include "log.h"
#include "event_dispatcher.h"
#include "channel.h"
#include "utils.h"

// 初始化event_loop
EvLoopPtr event_loop::event_loop_init(const std::string& _threadName) 
{
    auto eventLoop = std::make_shared<event_loop>();
    eventLoop->init(_threadName);
    return eventLoop;
}

void event_loop::init(const std::string& _threadName)
{
    threadName = _threadName;
    ownThreadID = std::this_thread::get_id();
    evDispatchPtr = std::make_shared<event_dispatcher>();
    evDispatchPtr->init(shared_from_this()); 
}

void event_loop::event_loop_add_channel_event(EvLoopPtr& eventLoop, int fd, const ChanPtr& channel)
{
    // 线程使用队列改为非阻塞，add 不一定是相同线程
    eventLoop->evDispatchPtr->addToQueue(channel);
    yolanda_msgx("eventloop add channel thread:%s", eventLoop->threadName.c_str());
}

void event_loop::event_loop_remove_channel_event(EvLoopPtr& eventLoop, int fd, const ChanPtr& channel) 
{
    assertInSameThread(eventLoop);
    channel->isTerminate = true;
    yolanda_msgx("eventloop remove channel thread:%s", eventLoop->threadName.c_str());
}

void event_loop::event_loop_update_channel_event(EvLoopPtr& eventLoop, int fd, const ChanPtr& channel) 
{
    assertInSameThread(eventLoop);
    eventLoop->evDispatchPtr->event_update(channel);
    yolanda_msgx("eventloop update channel thread:%s", eventLoop->threadName.c_str());
}

/*
 * 1.参数验证
 * 2.调用dispatcher来进行事件分发,分发完回调事件处理函数
 */
void event_loop::event_loop_run() 
{
    if (this->ownThreadID != std::this_thread::get_id())
        exit(1);

    yolanda_msgx("event loop run, %s", this->threadName.c_str());
    while (!this->quit) 
	this->evDispatchPtr->event_dispatch();

    yolanda_msgx("event loop end, %s", this->threadName.c_str());
}

int event_loop::get_event_size()
{
    if(!evDispatchPtr)
	return -1;
    return evDispatchPtr->get_size();
}

