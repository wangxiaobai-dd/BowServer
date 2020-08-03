add_executable(MainServer MainServer.cpp)
target_link_libraries(MainServer PRIVATE yolanda)
target_link_libraries(MainServer PRIVATE spdlog::spdlog)
