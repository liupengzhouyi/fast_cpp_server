cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (BUILD_MY_ARG_PARSER)
    message("BUILD_MY_ARG_PARSER is ON")
    print_colored_message("------------------------------" COLOR magenta)
    print_colored_message("Building my_arg_parser library..." COLOR yellow)
    
    set(MY_ARG_PARSER_INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/src/util/my_arg_parser)
    file(GLOB_RECURSE MY_ARG_PARSER_SOURCES ${PROJECT_SOURCE_DIR}/src/util/my_arg_parser/*.cpp)

    pretty_print_list("MYCONFIG_INCLUDE_DIRECTORIES List" MY_ARG_PARSER_INCLUDE_DIRECTORIES)
    pretty_print_list("MYCONFIG_SOURCES List" MY_ARG_PARSER_SOURCES)

    add_library(my_arg_parser STATIC ${MY_ARG_PARSER_SOURCES})
    target_include_directories(my_arg_parser PUBLIC ${MY_ARG_PARSER_INCLUDE_DIRECTORIES})

    print_colored_message("Building my_arg_parser library over." COLOR yellow)
    print_colored_message("------------------------------" COLOR magenta)
else()
    message("BUILD_MY_ARG_PARSER is OFF, skipping my_config library build")
    return()
endif()

