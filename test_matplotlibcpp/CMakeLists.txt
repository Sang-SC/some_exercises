cmake_minimum_required(VERSION 3.12)

project(TestMatplotlibCpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

add_compile_options(-Wall)

include_directories(
    /usr/include/python3.8
)

# 使用matplotlibcpp需要链接python3.8
link_directories(
    /usr/lib/python3.8
)

add_executable(1_first_program src/1_first_program.cpp)
target_link_libraries(1_first_program python3.8)
target_include_directories(1_first_program PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(2_test_animation src/2_test_animation.cpp)
target_link_libraries(2_test_animation python3.8)
target_include_directories(2_test_animation PRIVATE ${CMAKE_SOURCE_DIR}/include)