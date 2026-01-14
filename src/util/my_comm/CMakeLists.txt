cmake_minimum_required(VERSION 3.10)

message("CMAKE_VERSION: ${CMAKE_VERSION}")



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (BUILD_MY_COMM)
    message("BUILD_MY_COMM is ON")
    print_colored_message("------------------------------" COLOR magenta)
    print_colored_message("Building my_comm library..." COLOR yellow)
    
    set(MY_COMM_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/util/my_comm)
    file(GLOB_RECURSE MY_COMM_SOURCES ${PROJECT_SOURCE_DIR}/src/util/my_comm/*.cpp)

    pretty_print_list("MY_COMM_INCLUDE_DIRECTORIES List" MY_COMM_INCLUDE_DIRECTORIES)
    pretty_print_list("MY_COMM_SOURCES List" MY_COMM_SOURCES)

    add_library(my_comm STATIC ${MY_COMM_SOURCES})
    target_include_directories(my_comm PUBLIC ${MY_COMM_INCLUDE_DIRECTORIES})
    target_link_libraries(my_comm PUBLIC pthread)
    target_link_libraries(my_comm PUBLIC mylog)
    target_link_libraries(my_comm PUBLIC myconfig)
    print_colored_message("Building my_comm library over." COLOR yellow)
    print_colored_message("------------------------------" COLOR magenta)
else()
    message("BUILD_MY_COMM is OFF, skipping my_comm library build")
    return()
endif()

