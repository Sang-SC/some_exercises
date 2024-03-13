#include "logger_config.h"

// 定义成全局变量，方便其他文件调用
std::shared_ptr<spdlog::logger> logger = nullptr;

LoggerConfig::LoggerConfig() {
    logger = spdlog::stdout_color_mt("logger");
    logger->set_level(spdlog::level::debug);
    logger->set_pattern("[%H:%M:%S.%f] [%ius] [%^%l%$] %v");
    logger->debug("spdlog logger config success.");
}

LoggerConfig::~LoggerConfig() {}

static LoggerConfig LoggerConfig1;