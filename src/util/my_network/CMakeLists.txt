cmake_minimum_required(VERSION 3.10)

message("CMAKE_VERSION: ${CMAKE_VERSION}")



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (BUILD_MY_NETWORK)
    message("BUILD_MY_NETWORK is ON")
    print_colored_message("------------------------------" COLOR magenta)
    print_colored_message("Building my_network library..." COLOR yellow)
    
    set(MY_NETWORK_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/util/my_network)
    file(GLOB_RECURSE MY_NETWORK_SOURCES ${PROJECT_SOURCE_DIR}/src/util/my_network/*.cpp)

    pretty_print_list("MY_NETWORK_INCLUDE_DIRECTORIES List" MY_NETWORK_INCLUDE_DIRECTORIES)
    pretty_print_list("MY_NETWORK_SOURCES List" MY_NETWORK_SOURCES)

    add_library(my_network STATIC ${MY_NETWORK_SOURCES})
    target_include_directories(my_network PUBLIC ${MY_NETWORK_INCLUDE_DIRECTORIES})
    target_link_libraries(my_network PUBLIC pthread)
    target_link_libraries(my_network PUBLIC mylog)
    target_link_libraries(my_network PUBLIC myconfig)
    print_colored_message("Building my_network library over." COLOR yellow)
    print_colored_message("------------------------------" COLOR magenta)
else()
    message("BUILD_MY_NETWORK is OFF, skipping my_network library build")
    return()
endif()

