# spdlog全局日志配置的库文件
add_library(logger_config STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/logger_config.cc)
target_include_directories(logger_config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# cpptime 头文件相关配置，除此之外还得让使用该头文件的文件链接 
find_package(Threads REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)