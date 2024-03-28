/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * STM32 端：需要烧录 test_serial_jitter 程序，效果是定时器 500Hz 回传 test_size 个字节。
 * @version 0.1
 * @date 2024-03-28
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "utils/logger_config.h"
#include "utils/real_time.h"
#include <libserial/SerialStream.h>
#include <string>

LibSerial::SerialStream serialPort;  // 串口对象

/**
 * @brief 波特率 921600 条件下，连续接收字节测试，使用个人 Ubuntu20.04 系统进行试验
 * stm32 500Hz 发送 1 个字符，则大约 2050, 2050, 3900, 2 依次循环
 * stm32 500Hz 发送 4 个字符，则大约 2000 + 3 个 2，4000 + 7 个 2，6000 + 11 个 2 随机出现
 * stm32 500Hz 发送 8 个字符，则大约 2000，4000，6000，8000随机出现
 * stm32 500Hz 发送 16 个字符，则 4000 稳定出现
 * stm32 500Hz 发送 32 个字符，则 2000 稳定出现，
 * stm32 500Hz 发送 33 个字符（也即下面的 i 改成 33，stm32 也改成 33）， 则会很规律的间隔 32 次出现一个 300 多微秒
 * stm32 500Hz 发送 34 个字符（也即下面的 i 改成 34，stm32 也改成 34）， 则会很规律的间隔 16 次出现一个 300 多微秒
 * 36 个字符则间隔 8 次，40 个字符则间隔 4 次，48 个字符则 2178us 和 1823us 循环（而非 2000us）
 * stm32 500Hz 发送 64 个字符，则 2000 稳定出现
 * stm32 500Hz 发送 96 个字符，则 2000 稳定出现
 * stm32 500Hz 发送 128 个字符，则 2000 稳定出现
 * stm32 500Hz 发送 129 个字符，则会隔 32 个冒出来 2300us
 * stm32 500Hz 发送 130 个字符，则会隔 16 个冒出来两个 2900us + 1400us
 * stm32 500Hz 发送 160 个字符，则 2000 稳定出现
 * stm32 500Hz 发送 192 个字符，则 4000 稳定出现
 * stm32 500Hz 发送 256 个字符，则 4000 稳定出现（奇怪，怎么不是 2000 了，难道是波特率太低了？）
 *
 * 结论：CH341 芯片内部有 32 个字节缓冲区，所以需要用 32 个字符的倍数通信。
 * 最多 160 个字符能 2ms 稳定接受字节（不发送字节的情况下）
 *
 */
void test() {
    while (1) {
        // logger->debug("---------------test：连续接收字节测试---------------");

        uint16_t test_size = 160;

        char ch;
        static uint64_t count = 0;
        static uint64_t count2 = 0;
        for (uint16_t i = 0; i < test_size; i++) {
            count++;
            serialPort.get(ch);
            // logger->debug("接收到字节：{}, count = {}", ch, count);
        }

        count2++;
        logger->info("接收一帧字节， count2 = {}", count2);
    }
}

int main(int argc, char **argv) {
    // // 设置进程为实时进程
    // if (SetProcessHighPriority(90) != 0) {
    //     logger->error("设置进程为实时进程失败");
    //     return -1;
    // }
    // logger->info("设置进程为实时进程成功，优先级为90");

    // 打开串口
    std::string portname = "/dev/stm32";
    serialPort.Open(portname);
    serialPort.SetBaudRate(LibSerial::BaudRate::BAUD_921600);
    // serialPort.SetBaudRate(LibSerial::BaudRate::BAUD_2000000);
    if (!serialPort.good()) {
        spdlog::error("无法打开串口");
        return -1;
    }
    logger->info("串口打开成功！");

    test();

    return 0;
}