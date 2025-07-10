cmake_minimum_required(VERSION 3.10)

message("CMAKE_VERSION: ${CMAKE_VERSION}")



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (BUILD_MY_SYSTEM_HEALTHY)
    message("BUILD_MY_SYSTEM_HEALTHY is ON")
    print_colored_message("------------------------------" COLOR magenta)
    print_colored_message("Building my_system_healthy library..." COLOR yellow)
    
    set(MY_SYSTEM_HEALTHY_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/util/my_system_healthy)
    file(GLOB_RECURSE MY_SYSTEM_HEALTHY_SOURCES ${PROJECT_SOURCE_DIR}/src/util/my_system_healthy/*.cpp)

    pretty_print_list("MY_SYSTEM_HEALTHY_INCLUDE_DIRECTORIES List" MY_SYSTEM_HEALTHY_INCLUDE_DIRECTORIES)
    pretty_print_list("MY_SYSTEM_HEALTHY_SOURCES List" MY_SYSTEM_HEALTHY_SOURCES)

    add_library(my_system_healthy STATIC ${MY_SYSTEM_HEALTHY_SOURCES})
    target_include_directories(my_system_healthy PUBLIC ${MY_SYSTEM_HEALTHY_INCLUDE_DIRECTORIES})
    target_link_libraries(my_system_healthy PUBLIC pthread)
    target_link_libraries(my_system_healthy PUBLIC mylog)
    target_link_libraries(my_system_healthy PUBLIC myproto)
    print_colored_message("Building my_system_healthy library over." COLOR yellow)
    print_colored_message("------------------------------" COLOR magenta)
else()
    message("BUILD my_system_healthy is OFF, skipping my_system_healthy library build")
    return()
endif()

