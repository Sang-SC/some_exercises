cmake_minimum_required(VERSION 3.14)

get_filename_component(PROJECT ${CMAKE_SOURCE_DIR} NAME)
project(PROJECT)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

# 将自己编写的子模块 utils 添加到工程中
add_subdirectory(utils)

# 添加第三方库 serial
find_library(LIBSERIAL_LIBRARIES NAMES serial)  

# 生成可执行文件
add_executable(test_serial_latency src/test_serial_latency.cpp)
target_link_libraries(test_serial_latency 
                      ${LIBSERIAL_LIBRARIES}
                      logger_config
                      real_time)

add_executable(test_serial_jitter src/test_serial_jitter.cpp)
target_link_libraries(test_serial_jitter 
                      ${LIBSERIAL_LIBRARIES}
                      logger_config
                      real_time)






