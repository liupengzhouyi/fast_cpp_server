cmake_minimum_required(VERSION 3.10)

# 从源文件列表中移除 main.cpp
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# 查找所有通用源文件，不包含 main.cpp
file(GLOB_RECURSE MYLIB_SOURCES
    ${PROJECT_SOURCE_DIR}/src/tools/simple_tcp_client/*.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/thread_safe_buffer/*.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/ping_tools/*.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/system_healthy/*.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/example/*.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/monitor/device_online_monitor/*.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/thread_safe_queue/*.cpp
)

# 创建静态库 mylib
add_library(mylib STATIC ${MYLIB_SOURCES})

set(MYLIB_INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}/src/tools/simple_tcp_client
    ${PROJECT_SOURCE_DIR}/src/tools/thread_safe_buffer
    ${PROJECT_SOURCE_DIR}/src/tools/ping_tools
    ${PROJECT_SOURCE_DIR}/src/tools/system_healthy
    ${PROJECT_SOURCE_DIR}/src/tools/example
    ${PROJECT_SOURCE_DIR}/src/tools/monitor/device_online_monitor
    ${PROJECT_SOURCE_DIR}/src/tools/thread_safe_queue
)

# 头文件路径
target_include_directories(mylib PUBLIC ${MYLIB_INCLUDE_DIRECTORIES})

# 如果你有条件判断 ENABLE_COVERAGE，可以在此处添加
if (ENABLE_COVERAGE)
    target_compile_options(mylib PRIVATE --coverage)
    target_link_libraries(mylib PUBLIC --coverage)
endif()

# 链接其他外部库
# 在根 CMakeLists.txt 中已经 add_subdirectory
print_colored_message("------------------------------" COLOR magenta)
print_colored_message("Adding mylib library with sources ..." COLOR yellow)
pretty_print_list("MYLIB_SOURCES List" MYLIB_SOURCES)
pretty_print_list("MYLIB_INCLUDE_DIRECTORIES List" MYLIB_INCLUDE_DIRECTORIES)

# # spdlog
# target_link_libraries(mylib PUBLIC spdlog)
target_link_libraries(mylib PUBLIC nlohmann_json::nlohmann_json)
# protobuf
target_link_libraries(mylib PUBLIC protobuf::libprotobuf)
# mylog
target_link_libraries(mylib PUBLIC mylog)
# myconfig
target_link_libraries(mylib PUBLIC myconfig)
# myproto
target_link_libraries(mylib PUBLIC myproto)
# cpr
target_link_libraries(mylib PUBLIC cpr::cpr)
# zmq
target_link_libraries(mylib PUBLIC libzmq)
# pthread
target_link_libraries(mylib PUBLIC pthread)
# my_soft_healthy
target_link_libraries(mylib PRIVATE my_soft_healthy)

print_colored_message("Adding mylib library with sources over" COLOR yellow)
print_colored_message("------------------------------" COLOR magenta)