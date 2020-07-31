#ifndef CHANNEL_H
#define CHANNEL_H

#include <variant>
#include <functional>

#include "common.h"
#include "event_loop.h"
#include "buffer.h"

#define EVENT_TIMEOUT    0x01
/** Wait for a socket or FD to become readable */
#define EVENT_READ        0x02
/** Wait for a socket or FD to become writeable */
#define EVENT_WRITE    0x04
/** Wait for a POSIX signal to be raised*/
#define EVENT_SIGNAL    0x08

typedef std::variant<EvLoopPtr, TcpServerPtr, TcpConnPtr> VarData;
typedef std::function<int(const VarData& )> ChanCallbackFunc;

struct channel 
{
    int fd = -1;
    int events = -1;   // 表示event类型
    DataType dataType = NONE_TYPE;
    bool isTerminate = false;

    ChanCallbackFunc eventReadCallback;
    ChanCallbackFunc eventWriteCallback;
    VarData data; // callback data, 可能是event_loop，也可能是tcp_server或者tcp_connection

    static ChanPtr channel_new(int fd, int events, const ChanCallbackFunc& eventReadCallback, const ChanCallbackFunc& eventWriteCallback, const VarData& data, const DataType type);
    static bool channel_write_event_is_enabled(const ChanPtr& channel);
    static void channel_write_event_enable(const ChanPtr& channel);
    static void channel_write_event_disable(const ChanPtr& channel);
};

#endif
