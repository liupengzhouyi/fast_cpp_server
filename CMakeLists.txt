cmake_minimum_required(VERSION 3.10)

message("CMAKE_VERSION: ${CMAKE_VERSION}")

# 项目信息
project(fast_cpp_server_my)

message("PROJECT_NAME: ${PROJECT_NAME}")

message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 全局禁用 warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# 启用覆盖率
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

option(ENABLE_COVERAGE "Enable coverage reporting" ON)

if (ENABLE_COVERAGE)
    message(STATUS "Coverage enabled")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# 设置静态库和可执行文件的输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 静态库 (.a)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 动态库 (.so)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 可执行文件


function(pretty_print_list title list_var)
    # 定义颜色
    string(ASCII 27 Esc)
    set(ColorGreen "${Esc}[32m")
    set(ColorCyan  "${Esc}[36m")
    set(ColorReset "${Esc}[0m")

    # 打印标题
    message(STATUS "${ColorCyan}${title}:${ColorReset}")

    # 获取传入列表变量的内容
    set(list_content "${${list_var}}")

    # 找出最长项长度
    set(max_len 0)
    foreach(item ${list_content})
        string(LENGTH "${item}" len)
        if(len GREATER max_len)
            set(max_len ${len})
        endif()
    endforeach()

    # 打印每一项（对齐）
    foreach(item ${list_content})
        string(LENGTH "${item}" len)
        math(EXPR padding "${max_len} - ${len}")
        string(REPEAT " " ${padding} spaces)
        message(STATUS "${ColorGreen}item:${ColorReset} ${item}${spaces}")
    endforeach()
endfunction()

function(print_colored_message MESSAGE_STRING)
    set(options BOLD)
    set(oneValueArgs COLOR)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "${options}" "${oneValueArgs}" "")

    # ANSI escape codes
    string(ASCII 27 Esc)

    set(color_reset "${Esc}[0m")
    set(color_bold "${Esc}[1m")

    # Define color codes
    set(color_red "${Esc}[31m")
    set(color_green "${Esc}[32m")
    set(color_yellow "${Esc}[33m")
    set(color_blue "${Esc}[34m")
    set(color_magenta "${Esc}[35m")
    set(color_cyan "${Esc}[36m")
    set(color_white "${Esc}[37m")

    # Get selected color
    set(color_code "")
    if(ARG_COLOR STREQUAL "red")
        set(color_code "${color_red}")
    elseif(ARG_COLOR STREQUAL "green")
        set(color_code "${color_green}")
    elseif(ARG_COLOR STREQUAL "yellow")
        set(color_code "${color_yellow}")
    elseif(ARG_COLOR STREQUAL "blue")
        set(color_code "${color_blue}")
    elseif(ARG_COLOR STREQUAL "magenta")
        set(color_code "${color_magenta}")
    elseif(ARG_COLOR STREQUAL "cyan")
        set(color_code "${color_cyan}")
    elseif(ARG_COLOR STREQUAL "white")
        set(color_code "${color_white}")
    endif()

    # Add bold if requested
    if(ARG_BOLD)
        set(color_code "${color_bold}${color_code}")
    endif()

    # Print the colored message
    message("${color_code}${MESSAGE_STRING}${color_reset}")
endfunction()

# 添加protobuf库
set(protobuf_BUILD_TESTS OFF CACHE BOOL "Disable protobuf tests")
set(protobuf_USE_EXTERNAL_GTEST OFF CACHE BOOL "Disable external GTest usage")
add_subdirectory(external/protobuf)
# 假设你生成后的 protoc 可执行文件在 build/bin 目录中
set(Protobuf_PROTOC_EXECUTABLE $<TARGET_FILE:protoc>)
# set(Protobuf_PROTOC_EXECUTABLE ${PROJECT_SOURCE_DIR}/build/bin/protoc)
message(STATUS "Protobuf protoc executable: ${Protobuf_PROTOC_EXECUTABLE}")
# 添加easyloggingpp库
set(build_static_lib ON)
add_subdirectory(external/easyloggingpp)
# 设置spdlog为静态库
set(SPDLOG_BUILD_SHARED OFF)
# 添加spdlog库
add_subdirectory(external/spdlog)
# 添加日志模块
set(BUILD_MY_LOG ON CACHE BOOL "Build mylog library")
add_subdirectory(src/util/my_log)
# 添加nlohmann/json库
add_subdirectory(external/json)
# 添加 myconfig 库
set(BUILD_MY_CONFIG ON CACHE BOOL "Build my_config library")
add_subdirectory(src/util/my_config)
# 添加cpr库
set(CPR_USE_SYSTEM_CURL ON)
add_subdirectory(external/cpr)
# 添加mosquitto库
set(DOCUMENTATION OFF CACHE BOOL "Disable documentation generation")
add_subdirectory(external/mosquitto)
# 添加libzmq库
set(WITH_TLS OFF)
add_subdirectory(external/libzmq)
# 添加 myproto 库
set(BUILD_MY_PROTO ON CACHE BOOL "Build myproto library")
add_subdirectory(src/protobuf)
# 添加 mylib 库
set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage for mylib")
add_subdirectory(src)

set(THIRD_INCLUDE_DIRECTORIES 
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
    ${PROJECT_SOURCE_DIR}/external/libzmq/include
    ${PROJECT_SOURCE_DIR}/external/easyloggingpp/src
    ${PROJECT_SOURCE_DIR}/external/protobuf/src
)

set(MY_INCLUDE_DIRECTORIES "")
set(MIAN_SOURCE "${PROJECT_SOURCE_DIR}/src/main.cpp")

# 查找所有源文件
file(GLOB_RECURSE SOURCES "")

include_directories(
    ${THIRD_INCLUDE_DIRECTORIES}
)

print_colored_message("------------------------------" COLOR magenta)
print_colored_message("Building bin exec file ..." COLOR yellow)
# 添加可执行文件
add_executable(${PROJECT_NAME} ${MIAN_SOURCE} ${SOURCES})

# # 链接spdlog静态库，使用 PRIVATE 关键字
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
target_link_libraries(${PROJECT_NAME} PRIVATE mylog)
target_link_libraries(${PROJECT_NAME} PRIVATE myconfig)
target_link_libraries(${PROJECT_NAME} PRIVATE myproto)
target_link_libraries(${PROJECT_NAME} PRIVATE mylib)

add_dependencies(mylog spdlog::spdlog)
add_dependencies(mylib mylog)
add_dependencies(mylib myconfig)
add_dependencies(mylib myproto)
add_dependencies(mylib libzmq)
add_dependencies(mylib cpr::cpr)
add_dependencies(mylib nlohmann_json::nlohmann_json)

pretty_print_list("MIAN_SOURCE " MIAN_SOURCE)
pretty_print_list("MY_INCLUDE_DIRECTORIES List" MY_INCLUDE_DIRECTORIES)
pretty_print_list("SOURCES List" SOURCES)
pretty_print_list("THIRD_INCLUDE_DIRECTORIES List" THIRD_INCLUDE_DIRECTORIES)
print_colored_message("------------------------------" COLOR magenta)

# -------------------------------- Unit test ------------------------------
print_colored_message("------------------------------" COLOR magenta)
print_colored_message("Building Unit Test soft ..." COLOR yellow)
set(TEST_PROGRAM_NAME ${PROJECT_NAME}_Test)
# find all test source files
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
# 添加google test库
add_subdirectory(external/googletest)
pretty_print_list("TEST_SOURCES List" TEST_SOURCES)
# 添加测试
add_executable(${TEST_PROGRAM_NAME} ${TEST_SOURCES})
target_include_directories(
    ${TEST_PROGRAM_NAME} PRIVATE ${THIRD_INCLUDE_DIRECTORIES}
)

target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE gtest gtest_main)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE pthread)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE mylog)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE myconfig)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE myproto)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE mylib)

option(ENABLE_TEST "Enable unit testing" ON)
# start test
if(ENABLE_TEST) 
    enable_testing()
    add_test(NAME FastCppServerUnitTest COMMAND ${TEST_PROGRAM_NAME})
endif()
print_colored_message("------------------------------" COLOR magenta)

# -------------------------------- Coverage ------------------------------
# custom target for coverage
add_custom_target(coverage
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o coverage_report.html --exclude '${CMAKE_SOURCE_DIR}/external/.*'
    DEPENDS ${PROJECT_NAME}
)

# -------------------------------- CPack ------------------------------
# 启用 CPack 模块
include(CPack)

# 设置安装包的基本信息
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} - A high-performance C++ server")
set(CPACK_PACKAGE_VENDOR "YourCompany")
set(CPACK_PACKAGE_CONTACT "your_email@example.com")

# 设置安装包的生成格式（如 .tar.gz、.deb、.rpm 等）
set(CPACK_GENERATOR "TGZ") # 生成 .tar.gz 包
# 其他可选格式：ZIP、DEB、RPM 等

# 设置安装路径
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
set(SCRIPT_DIRECTORIES ${PROJECT_SOURCE_DIR}/scripts)

# 包含必要的文件
# 把 bin 下的所有文件都装到最终包里
# install(DIRECTORY ${CMAKE_BINARY_DIR}/bin/ DESTINATION bin)
# 不再批量拷贝 bin 目录，而是只安装你真正需要的可执行程序
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
# 如果你的单元测试可执行文件也想打包
install(TARGETS ${TEST_PROGRAM_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/config/ DESTINATION config)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/service/ DESTINATION service)
install(FILES ${SCRIPT_DIRECTORIES}/start.sh DESTINATION .)
install(FILES ${SCRIPT_DIRECTORIES}/install.sh DESTINATION .)
install(FILES ${SCRIPT_DIRECTORIES}/uninstall.sh DESTINATION .)