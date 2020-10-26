# BowServer
epoll 服务器
基于reactor 模式，主线程acceptor监听连接
非阻塞
多线程，每个线程可携带多个监听事件
语言：c++

log:spdlog  
增加了protobuf 自描述消息机制

客户端测试：
telnet 127.0.0.1 8800
或者Client/BowClient
