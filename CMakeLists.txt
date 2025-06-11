cmake_minimum_required(VERSION 3.10)

# 项目信息
project(fast_cpp_server)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

# 包含头文件目录
set(INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/include)

include_directories(
    ${INCLUDE_DIRECTORIES}
    ${PROJECT_SOURCE_DIR}/src/tools/simple_tcp_client
    ${PROJECT_SOURCE_DIR}/src/tools/thread_safe_buffer
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
)

# 设置静态库和可执行文件的输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 静态库 (.a)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 动态库 (.so)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 可执行文件

# # 强制设置 spdlog 的输出目录
# set_target_properties(spdlog::spdlog PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# # 强制设置 googletest 的输出目录
# set_target_properties(gtest PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set_target_properties(gtest_main PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置spdlog为静态库
set(SPDLOG_BUILD_SHARED OFF)

# 添加spdlog库
add_subdirectory(external/spdlog)

# 添加nlohmann/json库
add_subdirectory(external/json)

# 添加cpr库
set(CPR_USE_SYSTEM_CURL ON)
add_subdirectory(external/cpr)

set(protobuf_BUILD_TESTS OFF CACHE BOOL "Disable protobuf tests")
set(protobuf_USE_EXTERNAL_GTEST OFF CACHE BOOL "Disable external GTest usage")
add_subdirectory(external/protobuf)

set(DOCUMENTATION OFF CACHE BOOL "Disable documentation generation")
add_subdirectory(external/mosquitto)

add_subdirectory(external/libzmq)

# 查找所有源文件
file(
    GLOB_RECURSE
    SOURCES
    "src/*.cpp"
    "src/util/*.cpp"
    "src/tools/simple_tcp_client/*.cpp"
    "src/tools/thread_safe_buffer/*.cpp"
)

# 从源文件列表中移除 main.cpp
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

# 添加可执行文件
add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})

# 链接spdlog静态库，使用 PRIVATE 关键字
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(${PROJECT_NAME} PRIVATE cpr::cpr)

# 可执行文件链接到thread库
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

# -------------------------------- Unit test ------------------------------
set(TEST_PROGRAM_NAME ${PROJECT_NAME}_Test)
# find all test source files
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

# 添加google test库
add_subdirectory(external/googletest)

# 添加测试
add_executable(${TEST_PROGRAM_NAME} ${TEST_SOURCES} ${SOURCES})
target_include_directories(${TEST_PROGRAM_NAME} PRIVATE ${INCLUDE_DIRECTORIES})
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE gtest gtest_main)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE spdlog::spdlog)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE cpr::cpr)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE pthread)

# start test
enable_testing()
add_test(NAME FastCppServerUnitTest COMMAND ${TEST_PROGRAM_NAME})

# custom target for coverage
add_custom_target(coverage
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} --html --html-details -o coverage_report.html --exclude '${CMAKE_SOURCE_DIR}/external/.*'
    DEPENDS fast_cpp_server
)


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