#pragma once
#include <queue>
#include <memory>
#include <vector>
#include <mutex>

#include "common.h"
/*
 * 事件队列
 */
struct channel_queue
{
    channel_queue()
    {
	lock = std::unique_lock<std::mutex>(mutex, std::defer_lock);
    }
    std::mutex mutex;
    std::unique_lock<std::mutex> lock;
    std::queue<ChanPtr> queue;
    std::queue<ChanPtr> to_check_queue;

    // 主线程
    void addChan(const ChanPtr& channel)
    {
	lock.lock();
	queue.push(channel);
	++wait_size;
	lock.unlock();
    }
    // 子线程
    void checkChan()
    {
	// 加锁 主线程添加 子线程取出处理
	lock.lock();
	while(!queue.empty())
	{
	    to_check_queue.push(queue.front());
	    queue.pop();
	}
	wait_size = 0;
	lock.unlock();
	while(!to_check_queue.empty())
	{
	    auto channel = to_check_queue.front();
	    to_check_queue.pop();
	    event_add(channel);
	}
    }
    virtual void event_add(const ChanPtr& channel) = 0;
    int wait_size = 0; // 这里不知道对否?
};

/*
 * epoll 事件分发器
 */
struct event_dispatcher : public channel_queue
{
    int epfd = -1; // epoll 实例
    std::vector<struct epoll_event> retEventVec;
    std::vector<ChanPtr> channelVec; // 任务列表
    std::unordered_map<int, ChanPtr> channelMap; // <fd, channel> 映射
    int channelSize = 0;

    void addToQueue(const ChanPtr& channel);
    
    void init(const EvLoopPtr& _evLoop);
    void event_add(const ChanPtr& channel) override;
    void event_del(const ChanPtr& channel);
    void event_update(const ChanPtr& channel);
    void event_dispatch();
    
    int get_size(){ return wait_size + channelSize; }
};
