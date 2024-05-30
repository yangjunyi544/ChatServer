cmake_minimum_required(VERSION 3.0)  # 要求cmake最低的版本号

project (demo) # 定义当前工程名字

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

set(SRC_LIST muduo_server.cpp)

add_executable(server ${SRC_LIST})

target_link_libraries (server muduo_net muduo_base pthread) # 执行可执行文件需要依赖的库