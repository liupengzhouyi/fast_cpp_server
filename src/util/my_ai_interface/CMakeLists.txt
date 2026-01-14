cmake_minimum_required(VERSION 3.10)

message("CMAKE_VERSION: ${CMAKE_VERSION}")



set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (BUILD_MY_AI_INTERFACE)
    message("BUILD_MY_AI_INTERFACE is ON")
    print_colored_message("------------------------------" COLOR magenta)
    print_colored_message("Building my_ai_interface library..." COLOR yellow)
    
    set(MY_AI_INTERFACE_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/util/my_ai_interface)
    file(GLOB_RECURSE MY_AI_INTERFACE_SOURCES ${PROJECT_SOURCE_DIR}/src/util/my_ai_interface/*.cpp)

    pretty_print_list("MY_AI_INTERFACE_INCLUDE_DIRECTORIES List" MY_AI_INTERFACE_INCLUDE_DIRECTORIES)
    pretty_print_list("MY_AI_INTERFACE_SOURCES List" MY_AI_INTERFACE_SOURCES)

    add_library(my_ai_interface STATIC ${MY_AI_INTERFACE_SOURCES})
    target_include_directories(my_ai_interface PUBLIC ${MY_AI_INTERFACE_INCLUDE_DIRECTORIES})
    target_link_libraries(my_ai_interface PUBLIC pthread)
    target_link_libraries(my_ai_interface PUBLIC mylog)
    target_link_libraries(my_ai_interface PUBLIC myconfig)
    print_colored_message("Building my_ai_interface library over." COLOR yellow)
    print_colored_message("------------------------------" COLOR magenta)
else()
    message("BUILD_MY_AI_INTERFACE is OFF, skipping my_ai_interface library build")
    return()
endif()

