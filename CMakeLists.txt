cmake_minimum_required(VERSION 3.10)

# 项目信息
project(FastCppServer)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 启用覆盖率
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

option(ENABLE_COVERAGE "Enable coverage reporting" ON)

if (ENABLE_COVERAGE)
    message(STATUS "Coverage enabled")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 设置spdlog为静态库
set(SPDLOG_BUILD_SHARED OFF)

# 添加spdlog库
add_subdirectory(external/spdlog)

# 添加nlohmann/json库
add_subdirectory(external/json)

# 添加google test库
add_subdirectory(external/googletest)

# 查找所有源文件
file(GLOB_RECURSE SOURCES "src/*.cpp" "util/*.cpp")
# 从源文件列表中移除 main.cpp
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# 添加可执行文件
add_executable(FastCppServer src/main.cpp ${SOURCES})

# 链接spdlog静态库，使用 PRIVATE 关键字
target_link_libraries(FastCppServer PRIVATE spdlog::spdlog)

target_link_libraries(FastCppServer PRIVATE nlohmann_json::nlohmann_json)


# -------------------------------- Unit test ------------------------------
# find all test source files
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

# 添加测试
add_executable(FastCppServerTest ${TEST_SOURCES} ${SOURCES})
target_include_directories(FastCppServerTest PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(FastCppServerTest PRIVATE gtest gtest_main)

# start test
enable_testing()
add_test(NAME FastCppServerUnitTest COMMAND FastCppServerTest)

# custom target for coverage
add_custom_target(coverage
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o coverage_report.html --exclude '${CMAKE_SOURCE_DIR}/external/.*'
    DEPENDS FastCppServerTest
)

# # 安装可执行文件
# install(TARGETS FastCppServer DESTINATION bin)

# # 安装头文件
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

# # 包含CPack模块
# include(CPack)