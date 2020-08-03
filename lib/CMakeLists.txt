set(yolanda_lib
	#log.cpp
	read.cpp
	tcp_server.cpp
	tcp_client.cpp
	event_loop.cpp
	channel.cpp
	acceptor.cpp
	thread_pool.cpp
	event_loop_thread.cpp
	utils.cpp
	event_dispatcher.cpp
	buffer.cpp
	tcp_connection.cpp
	)


add_library(yolanda STATIC ${yolanda_lib})
target_link_libraries(yolanda PRIVATE pthread)
target_link_libraries(yolanda PRIVATE spdlog::spdlog)

install(TARGETS yolanda DESTINATION lib)

