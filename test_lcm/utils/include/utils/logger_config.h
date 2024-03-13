#pragma once
/**
 * @file logger_config.h
 * @author your name (you@domain.com)
 * @brief 依赖于 spdlog 日志库，自定义配置格式，并定义全局变量 logger 可在其他文件中使用
 * @version 0.1
 * @date 2024-03-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <spdlog/spdlog.h>                    // spdlog日志库，主头文件
#include <spdlog/sinks/stdout_color_sinks.h>  // spdlog日志库，多线程彩色控制台输出头文件
#include <spdlog/fmt/ostr.h>                  // spdlog日志库，fmt格式化输出vector向量要用到的头文件
#include <memory>                             // std::shared_ptr 要用的头文件

extern std::shared_ptr<spdlog::logger> logger;

class LoggerConfig {
   public:
    LoggerConfig();
    ~LoggerConfig();
};