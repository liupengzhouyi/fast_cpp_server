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

# --------------------- OpenCV 设置 ---------------------
# 指定需要构建的模块（可按需调整）,

set(BUILD_LIST calib3d,core,dnn,features2d,flann,gapi,highgui,imgcodecs,imgproc,ml,objdetect,photo,stitching,video,videoio)
# 是否构建为静态库
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libs")
# opencv contrib 路径（可选）
set(OPENCV_EXTRA_MODULES_PATH ${CMAKE_SOURCE_DIR}/external/opencv_contrib/modules)
set(WITH_PROTOBUF OFF CACHE BOOL "Don't use protobuf in OpenCV")
# 添加 OpenCV 子目录
add_subdirectory(external/opencv)

# --------------------- Eigen 设置 ---------------------
# Eigen 是头文件库，不需要编译，只需添加路径
set(EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/eigen)
include_directories(${EIGEN3_INCLUDE_DIR})

# --------------------- SymEngine 设置 ---------------------
# 启用静态构建（可选）
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static symengine")
set(HAVE_SYMENGINE_GMP OFF CACHE BOOL "Not use GMP")
set(SYMENGINE_USE_MPFR OFF CACHE BOOL "")
set(SYMENGINE_USE_MPC OFF CACHE BOOL "")
set(SYMENGINE_USE_FLINT OFF CACHE BOOL "")
# 关闭测试与示例
set(BUILD_TESTS OFF CACHE BOOL "")
set(BUILD_BENCHMARKS OFF CACHE BOOL "")
set(BUILD_EXAMPLES OFF CACHE BOOL "")
add_subdirectory(external/symengine)

# 添加 myproto 库
set(BUILD_MY_PROTO ON CACHE BOOL "Build myproto library")
add_subdirectory(src/protobuf)
# 添加 my_arg_parser 库
set(BUILD_MY_ARG_PARSER ON CACHE BOOL "Build my_arg_parser library")
add_subdirectory(src/util/my_arg_parser)
# 添加 my_soft_healthy 库
set(BUILD_MY_SOFT_HEALTHY ON CACHE BOOL "Build my_soft_healthy library")
add_subdirectory(src/util/my_soft_healthy)
# 添加 my_system_healthy 库
set(BUILD_MY_SYSTEM_HEALTHY ON CACHE BOOL "Build my_system_healthy library")
add_subdirectory(src/util/my_system_healthy)
# 添加 mylib 库
set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage for mylib")
add_subdirectory(src)

set(THIRD_INCLUDE_DIRECTORIES 
    ${PROJECT_SOURCE_DIR}/external/spdlog/include
    ${PROJECT_SOURCE_DIR}/external/libzmq/include
    ${PROJECT_SOURCE_DIR}/external/easyloggingpp/src
    ${PROJECT_SOURCE_DIR}/external/protobuf/src
    ${PROJECT_SOURCE_DIR}/external/eigen                            # Eigen 是头文件-only
    ${PROJECT_SOURCE_DIR}/external/symengine
    ${PROJECT_SOURCE_DIR}/external/opencv/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/calib3d/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/core/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/dnn/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/features2d/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/flann/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/gapi/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/highgui/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/imgcodecs/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/imgproc/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/ml/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/objdetect/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/photo/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/stitching/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/video/include
    ${PROJECT_SOURCE_DIR}/external/opencv/modules/videoio/include
    ${PROJECT_SOURCE_DIR}/build                                     # OpenCV 编译后的头文件路径
    ${PROJECT_SOURCE_DIR}/build/external/symengine                  # SymEngine 编译后的头文件路径
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
target_link_libraries(${PROJECT_NAME} PRIVATE my_arg_parser)
target_link_libraries(${PROJECT_NAME} PRIVATE my_soft_healthy)
target_link_libraries(${PROJECT_NAME} PRIVATE my_system_healthy)
target_link_libraries(${PROJECT_NAME} PRIVATE mylib)
# 链接 OpenCV 库
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_calib3d)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_core)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_dnn)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_features2d)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_flann)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_gapi)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_highgui)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_imgcodecs)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_imgproc)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_ml)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_objdetect)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_photo)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_stitching)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_video)
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_videoio)

# 链接 Engine（Eigen 是头文件库，不需要 link）
# 链接 SymEngine
target_link_libraries(${PROJECT_NAME} PRIVATE symengine)
# # 包含 Eigen 头文件
# target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN3_INCLUDE_DIR})

add_dependencies(mylog spdlog::spdlog)
add_dependencies(mylib mylog)
add_dependencies(mylib myconfig)
add_dependencies(mylib myproto)
add_dependencies(mylib my_soft_healthy)
add_dependencies(mylib libzmq)
add_dependencies(mylib cpr::cpr)
add_dependencies(mylib nlohmann_json::nlohmann_json)
add_dependencies(mylib symengine)
# # 依赖 opencv lib
# add_dependencies(mylib opencv_calib3d)
# add_dependencies(mylib opencv_core)
# add_dependencies(mylib opencv_dnn)
# add_dependencies(mylib opencv_features2d)
# add_dependencies(mylib opencv_flann)
# add_dependencies(mylib opencv_gapi)
# add_dependencies(mylib opencv_highgui)
# add_dependencies(mylib opencv_imgcodecs)
# add_dependencies(mylib opencv_imgproc)
# add_dependencies(mylib opencv_ml)
# add_dependencies(mylib opencv_objdetect)
# add_dependencies(mylib opencv_photo)
# add_dependencies(mylib opencv_stitching)
# add_dependencies(mylib opencv_video)
# add_dependencies(mylib opencv_videoio)


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
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE my_arg_parser)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE mylib)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE my_soft_healthy)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE my_system_healthy)
# 链接 OpenCV 库
# 链接 OpenCV 库
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_calib3d)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_core)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_dnn)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_features2d)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_flann)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_gapi)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_highgui)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_imgcodecs)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_imgproc)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_ml)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_objdetect)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_photo)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_stitching)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_video)
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE opencv_videoio)
# 链接 SymEngine（Eigen 是头文件库，不需要 link）
target_link_libraries(${TEST_PROGRAM_NAME} PRIVATE symengine)
# 包含 Eigen 头文件
target_include_directories(${TEST_PROGRAM_NAME} PRIVATE ${EIGEN3_INCLUDE_DIR})


include(GoogleTest)
gtest_discover_tests(${TEST_PROGRAM_NAME}
    TEST_PREFIX "${TEST_PROGRAM_NAME}."
    DISCOVERY_TIMEOUT 30
)

option(ENABLE_TEST "Enable unit testing" ON)
# start test
if(ENABLE_TEST) 
    enable_testing()
    add_test(NAME ${PROJECT_NAME}UnitTest COMMAND ${TEST_PROGRAM_NAME})
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