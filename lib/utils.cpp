#include "utils.h"

#include <memory>

void assertInSameThread(const EvLoopPtr& eventLoop)
{
    if (eventLoop->ownThreadID != std::this_thread::get_id()) 
    {
	spdlog::error("not in the same thread");
        exit(-1);
    }
}

// 1:same thread: 0:not the same thread
int isInSameThread(const EvLoopPtr& eventLoop)
{
    return eventLoop->ownThreadID == std::this_thread::get_id();
}
