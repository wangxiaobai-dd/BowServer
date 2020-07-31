#include "utils.h"
#include "log.h"

#include <memory>
#include <iostream>

void assertInSameThread(const EvLoopPtr& eventLoop)
{
    if (eventLoop->ownThreadID != std::this_thread::get_id()) 
    {
	std::cout << "ownID: " << eventLoop->ownThreadID << " get: " << std::this_thread::get_id() << std::endl;
        LOG_ERR("not in the same thread");
        exit(-1);
    }
}

// 1:same thread: 0:not the same thread
int isInSameThread(const EvLoopPtr& eventLoop)
{
    return eventLoop->ownThreadID == std::this_thread::get_id();
}
