cmake_minimum_required(VERSION 3.10)

message("CMAKE_VERSION: ${CMAKE_VERSION}")

message("Building my_proto library...")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


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

# 添加 proto 编译自动规则
# file(GLOB PROTO_FILES "${PROJECT_SOURCE_DIR}/src/proto/*.proto")
# message("---- PROTO_FILES: ${PROTO_FILES}")

if (BUILD_MY_PROTO)
    message("BUILD_MY_PROTO is ON")
    print_colored_message("------------------------------" COLOR magenta)
    print_colored_message("Adding myproto library with sources" COLOR yellow)
    set(PORTOBUF_INCLUDE_DIRECTORIES
        ${PROJECT_SOURCE_DIR}/src/protobuf
    )

    file(GLOB PROTO_FILES "${PROJECT_SOURCE_DIR}/src/proto/*.proto")

    set(GENERATED_PROTO_SRCS "")
    set(GENERATED_PROTO_HDRS "")

    foreach(PROTO_FILE ${PROTO_FILES})
        get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
        list(APPEND GENERATED_PROTO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_NAME}.pb.cc")
        list(APPEND GENERATED_PROTO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_NAME}.pb.h")

        add_custom_command(
            OUTPUT
                ${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_NAME}.pb.cc
                ${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_NAME}.pb.h
            COMMAND ${Protobuf_PROTOC_EXECUTABLE}
                --proto_path=${PROJECT_SOURCE_DIR}/src/proto
                --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}
                ${PROTO_FILE}
            DEPENDS ${PROTO_FILE}
            COMMENT "Generating C++ protobuf code from ${PROTO_FILE}"
            VERBATIM
        )
    endforeach()

    pretty_print_list("PROTO_FILES List" PROTO_FILES)
    pretty_print_list("GENERATED_PROTO_HDRS List" GENERATED_PROTO_HDRS)
    pretty_print_list("GENERATED_PROTO_SRCS List" GENERATED_PROTO_SRCS)

    add_custom_target(generate_protos ALL DEPENDS ${GENERATED_PROTO_SRCS})
    add_library(myproto STATIC ${GENERATED_PROTO_SRCS})
    add_dependencies(myproto generate_protos)

    target_include_directories(myproto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(myproto PUBLIC protobuf::libprotobuf)

    print_colored_message("BUILD_MY_PROTO is ON, myproto library built successfully" COLOR yellow)
    print_colored_message("------------------------------" COLOR magenta)
else()
    message("BUILD_MY_PROTO is OFF, skipping myproto library build")
    return()
endif()

