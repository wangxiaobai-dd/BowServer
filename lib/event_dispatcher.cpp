#include <vector>

#include <assert.h>
#include <sys/epoll.h>
#include "event_dispatcher.h"
#include "event_loop.h"
// #include "log.h"

void event_dispatcher::init(const EvLoopPtr& _evLoop)
{
    epfd = epoll_create1(0);
    if(epfd == -1)
    {
	spdlog::error("epoll create failed");
	return;
    }
    retEventVec.resize(LISTEN_SIZE);
}

void event_dispatcher::addToQueue(const ChanPtr& channel)
{
    this->addChan(channel);
    spdlog::info("chan push success");
}

void event_dispatcher::event_dispatch()
{
    // 防止cpu一直空跑
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    // 检查待处理连接
    checkChan();

    for(auto chanIter = channelList.begin(); chanIter != channelList.end();)
    {
	// 终止
	if((*chanIter)->isTerminate)
	{
	    spdlog::info("chan terminate success");
	    event_del(*chanIter);
	    channelMap.erase((*chanIter)->fd);
	    channelList.erase(chanIter++);
	    channelSize -= 1;
	}
	else
	    ++chanIter;
    }

    int ret = epoll_wait(epfd, &retEventVec[0], channelSize, 0);
    for (int i = 0; i < ret; i++) 
    {
	int fd = retEventVec[i].data.fd;
	assert(channelMap.count(fd));
	auto& channel = channelMap[fd];
    
        if (retEventVec[i].events & (EPOLLERR | EPOLLHUP))
	{
	    spdlog::error("epoll error");
	    event_del(channel);
	    channelMap.erase(fd);
	    channelList.remove(channel);
	    channelSize = channelList.size();
            continue;
        }

        if (retEventVec[i].events & EPOLLIN) 
	{
	    spdlog::info("event dispatcher read, {}", fd);
	    channel->eventReadCallback(channel->data);
        }

        if (retEventVec[i].events & EPOLLOUT) 
	{
	    spdlog::info("event dispatcher write, {}", fd);
	    channel->eventWriteCallback(channel->data);
        }
    }
}

void event_dispatcher::event_add(const ChanPtr& channel) 
{
    assert(epfd != -1 && channel->fd != -1);
    assert(!channelMap.count(channel->fd));

    int events = 0;
    if (channel->events & EVENT_READ) 
        events = events | EPOLLIN;
    if (channel->events & EVENT_WRITE) 
        events = events | EPOLLOUT;
    events |= EPOLLERR;

    struct epoll_event event;
    event.data.fd = channel->fd;
    event.events = events;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, channel->fd, &event) == -1) 
	spdlog::error("epoll_ctl add fd failed, errno:{}", errno);
    channelMap[channel->fd] = channel;
    channelList.push_back(channel);
    channelSize = channelList.size();
    if (channelSize > retEventVec.size())
	retEventVec.resize(channelSize + 10);
}

void event_dispatcher::event_del(const ChanPtr& channel)
{
    assert(epfd != -1 && channel->fd != -1);

    int events = 0;
    if (channel->events & EVENT_READ) 
        events |= EPOLLIN;
    if (channel->events & EVENT_WRITE)
	events |= EPOLLOUT;
    events |= EPOLLERR;

    struct epoll_event event;
    event.data.fd = channel->fd;
    event.events = events;
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, channel->fd, &event) == -1)
	spdlog::error("epoll_ctl delete fd failed, errno:{}", errno);
}

void event_dispatcher::event_update(const ChanPtr& channel)
{
    assert(epfd != -1 && channel->fd != -1);

    int events = 0;
    if (channel->events & EVENT_READ) 
        events |= EPOLLIN;
    if (channel->events & EVENT_WRITE)
        events |= EPOLLOUT;
    events |= EPOLLERR;

    struct epoll_event event;
    event.data.fd = channel->fd;
    event.events = events;
    if (epoll_ctl(epfd, EPOLL_CTL_MOD, channel->fd, &event) == -1) 
	spdlog::error("epoll_ctl mod fd failed, errno:{}", errno);
}

