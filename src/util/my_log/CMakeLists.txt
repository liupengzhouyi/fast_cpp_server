cmake_minimum_required(VERSION 3.10)

message("CMAKE_VERSION: ${CMAKE_VERSION}")



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (BUILD_MY_LOG)
    message("BUILD_MY_LOG is ON")
    print_colored_message("------------------------------" COLOR magenta)
    print_colored_message("Building mylog library..." COLOR yellow)
    
    set(MYLOG_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/util/my_log)
    file(GLOB_RECURSE MYLOG_SOURCES ${PROJECT_SOURCE_DIR}/src/util/my_log/*.cpp)

    pretty_print_list("MYLOG_INCLUDE_DIRECTORIES List" MYLOG_INCLUDE_DIRECTORIES)
    pretty_print_list("MYLOG_SOURCES List" MYLOG_SOURCES)

    add_library(mylog STATIC ${MYLOG_SOURCES})
    target_include_directories(mylog PUBLIC ${MYLOG_INCLUDE_DIRECTORIES})
    target_link_libraries(mylog PUBLIC spdlog::spdlog)
    add_dependencies(mylog spdlog::spdlog)
    print_colored_message("Building mylog library over." COLOR yellow)
    print_colored_message("------------------------------" COLOR magenta)
else()
    message("BUILD_MY_LOG is OFF, skipping mylog library build")
    return()
endif()

