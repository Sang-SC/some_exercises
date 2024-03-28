cmake_minimum_required(VERSION 3.12)

project(test_cpptime)

find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)

# spdlog全局日志配置的库文件
add_library(logger_config SHARED ${CMAKE_CURRENT_SOURCE_DIR}/logger_config.cc)
target_link_libraries(logger_config spdlog::spdlog)
target_include_directories(logger_config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(main main.cpp)
target_link_libraries(main logger_config Threads::Threads)  # cpptime 库依赖于 pthread 库