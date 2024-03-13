#include <lcm/lcm-cpp.hpp>
#include "latency_lcm/test_latency.hpp"
#include "utils/logger_config.h"
#include "utils/cpptime.h"

#include <chrono>
#include <csignal>

// 全局变量
lcm::LCM lcm1;
latency_lcm::test_latency my_data;

// 信号处理函数
void signalHandler(int signal) {
    if (signal == SIGINT) {
        exit(-1);
    }
}

void TimerCallback(CppTime::timer_id) {
    //
    logger->info("LCM: Publish successfully!");
    auto time_current = std::chrono::high_resolution_clock::now();
    my_data.sec = std::chrono::duration_cast<std::chrono::seconds>(time_current.time_since_epoch()).count();
    my_data.nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(time_current.time_since_epoch() % std::chrono::seconds(1)).count();

    lcm1.publish("2_test_latency", &my_data);
}

int main(int argc, char** argv) {
    // 注册信号处理函数
    std::signal(SIGINT, signalHandler);

    // 检查 lcm 配置是否成功
    if (!lcm1.good()) {
        return 1;
    }

    // 定时器相关配置
    CppTime::Timer cpp_timer;
    cpp_timer.add(std::chrono::microseconds(0), TimerCallback, std::chrono::microseconds(1000));
    while (1) {
    }

    return 0;
}