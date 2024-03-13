
#include "cpptime.h"
#include "logger_config.h"
#include <csignal>

/**
 * @brief 如果接收到SIGINT信号（终止进程信号），程序退出
 *
 * @param signal
 */
void signalHandler(int signal) {
    if (signal == SIGINT) {
        exit(-1);
    }
}

void timerCallback1(CppTime::timer_id) {
    //
    logger->info("timerCallback1");
}

void timerCallback2(CppTime::timer_id) {
    //
    logger->info("timerCallback2");
}

void timerCallback3(CppTime::timer_id) {
    static long cnt = 0;
    if (cnt++ % 10 == 0) {
        logger->debug("timerCallback3 cnt: {}", cnt);
    }
}

void timerCallback4(CppTime::timer_id) {
    //
    logger->info("timerCallback4");
}

int main() {
    // 注册信号处理函数
    std::signal(SIGINT, signalHandler);

    // 初始化定时器，CppTime::Timer 是 cpptime.h 中定义的一个类
    CppTime::Timer cpp_timer;

    // // 测试1：添加两个一次性定时器，一个定时器 1s 后触发，一个定时器 3s 后触发
    // cpp_timer.add(std::chrono::microseconds(1000000), timerCallback1);
    // cpp_timer.add(std::chrono::seconds(3), timerCallback2);

    // 测试2：添加两个周期定时器，一个定时器周期为 1ms，一个定时器周期为 2ms
    cpp_timer.add(std::chrono::microseconds(0), timerCallback3, std::chrono::microseconds(1000));
    cpp_timer.add(std::chrono::microseconds(0), timerCallback4, std::chrono::microseconds(2000));

    while (1) {
    }

    return 0;
}