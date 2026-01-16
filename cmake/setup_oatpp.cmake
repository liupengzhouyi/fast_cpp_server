# cmake/setup_oatpp.cmake

# 1. 基础配置
set(OATPP_BUILD_TESTS ON CACHE BOOL "" FORCE)
set(OATPP_INSTALL OFF CACHE BOOL "" FORCE)

# 2. 编译核心
add_subdirectory(external/oatpp)

# 3. 内存伪造与 Config 文件模拟
set(oatpp_VERSION "1.4.0")
set(oatpp_FOUND TRUE)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/external/oatpp/oatppConfig.cmake" "# Dummy")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/external/oatpp/oatppConfigVersion.cmake" 
     "set(PACKAGE_VERSION \"1.4.0\")\nset(PACKAGE_VERSION_COMPATIBLE TRUE)")
set(oatpp_DIR "${CMAKE_CURRENT_BINARY_DIR}/external/oatpp")

# 4. 别名建立
if(TARGET oatpp)
    add_library(oatpp::oatpp ALIAS oatpp)
endif()
if(TARGET oatpp-test)
    add_library(oatpp::oatpp-test ALIAS oatpp-test)
endif()

# 5. 编译 Swagger
add_subdirectory(external/oatpp-swagger)

# 6. Swagger 别名
if(TARGET oatpp-swagger)
    add_library(oatpp::oatpp-swagger ALIAS oatpp-swagger)
endif()