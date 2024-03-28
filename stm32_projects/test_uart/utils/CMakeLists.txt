# 自定义 spdlog 日志记录器
find_package(spdlog REQUIRED)
add_library(logger_config STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/logger_config.cc)
target_include_directories(logger_config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 设置实时优先级的库文件
add_library(real_time STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/real_time.cc)
target_include_directories(real_time PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)