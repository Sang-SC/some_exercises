cmake_minimum_required(VERSION 3.12)

project(TestLcm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

add_compile_options(-Wall)

find_package(lcm REQUIRED)
find_package(spdlog REQUIRED)
find_package(rclcpp REQUIRED)

include(${LCM_USE_FILE})

lcm_wrap_types(
    CPP_HEADERS cpp_headers
    1_first_program/example_t.lcm
    2_test_latency/test_latency.lcm
)

lcm_add_library(messages_cpp CPP ${cpp_headers})
target_include_directories(messages_cpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

add_subdirectory(utils)
add_subdirectory(1_first_program)
add_subdirectory(2_test_latency)
add_subdirectory(3_compare_with_ROS2)




