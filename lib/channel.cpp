#include "channel.h"

ChanPtr channel::channel_new(int fd, int events, const ChanCallbackFunc& eventReadCallback, const ChanCallbackFunc& eventWriteCallback, const VarData& data, const DataType type) 
{
    auto chan = std::make_shared<channel>();
    chan->fd = fd;
    chan->events = events;
    chan->eventReadCallback = eventReadCallback;
    chan->eventWriteCallback = eventWriteCallback;
    chan->data = data;
    chan->dataType = type;
    return chan;
}

bool channel::channel_write_event_is_enabled(const ChanPtr& channel) 
{
    return channel->events & EVENT_WRITE;
}

void channel::channel_write_event_enable(const ChanPtr& channel) 
{
    if(channel->dataType != EVLOOP_TYPE)
	return;
    auto eventLoop = std::get<EvLoopPtr>(channel->data);
    channel->events |= EVENT_WRITE;
    event_loop::event_loop_update_channel_event(eventLoop, channel->fd, channel);
}

void channel::channel_write_event_disable(const ChanPtr& channel) 
{
    if(channel->dataType != EVLOOP_TYPE)
	return;
    auto eventLoop = std::get<EvLoopPtr>(channel->data);
    channel->events = channel->events & ~EVENT_WRITE;
    event_loop::event_loop_update_channel_event(eventLoop, channel->fd, channel);
}
