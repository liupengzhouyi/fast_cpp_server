cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (BUILD_MY_CONFIG)
    message("BUILD_MY_CONFIG is ON")
    print_colored_message("------------------------------" COLOR magenta)
    print_colored_message("Building my_config library..." COLOR yellow)
    
    set(MYCONFIG_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/util/my_config)
    file(GLOB_RECURSE MYCONFIG_SOURCES ${PROJECT_SOURCE_DIR}/src/util/my_config/*.cpp)

    pretty_print_list("MYCONFIG_INCLUDE_DIRECTORIES List" MYCONFIG_INCLUDE_DIRECTORIES)
    pretty_print_list("MYCONFIG_SOURCES List" MYCONFIG_SOURCES)
    # add_library(myconfig STATIC MyConfig.cpp)
    # target_include_directories(myconfig PUBLIC ${PROJECT_SOURCE_DIR}/src/util/my_config)

    add_library(myconfig STATIC ${MYCONFIG_SOURCES})
    target_include_directories(myconfig PUBLIC ${MYCONFIG_INCLUDE_DIRECTORIES})

    target_link_libraries(myconfig PUBLIC nlohmann_json::nlohmann_json yaml-cpp)
    print_colored_message("Building my_config library over." COLOR yellow)
    print_colored_message("------------------------------" COLOR magenta)
else()
    message("BUILD_MY_CONFIG is OFF, skipping my_config library build")
    return()
endif()

