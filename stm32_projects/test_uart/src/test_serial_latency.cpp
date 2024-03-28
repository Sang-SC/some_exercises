/**
 * @file test_serial_latency.cpp
 * @author your name (you@domain.com)
 * @brief
 * PC 端：
 * STM32 端：需要烧录 test_serial_latency 程序，效果是接受 PC 发送的 test_size 个字节，接收完立刻回传
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
#include <random>

LibSerial::SerialStream serialPort;  // 串口对象

/**
 * @brief 生成指定范围的随机整数，用于确保返回的字节和发送的字节相同
 *
 * @return int
 */
int GenerateRandomNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);
    return dis(gen);
}

/**
 * @brief 发送和接收字节延迟测试
 * ---------------波特率 921600 条件下，个人 Ubuntu20.04 系统进行试验---------------
 * 32 字节实测发送延迟 61us，接收延迟 800us
 * 64 字符实测发送延迟 100us，接收延迟 1500us.
 * 96 字符实测发送延迟 500us，接收延迟 1800us
 * 128 字符实测发送延迟 900us，接收延迟 2100us, 然而报错“接收到的字节和发送的字节不一致！”(不过 STM32 复位一次又好了)
 * 160 字符实测发送延迟 1300us，接收延迟 2400us
 * 192 字符实测发送延迟 1650us，接收延迟 2850us
 *
 *
 * ---------------波特率 1500000 条件下，个人 Ubuntu20.04 系统进行试验---------------
 * 实测 64 字节发送延迟 100us，接收延迟 950us
 * 理论计算可得, 128 * 10 / 1500000 = 0.853ms, 实测结果和最理想的结果相差不大
 * 实测 160 字节发送延迟 900us，接收延迟 1550us
 * 理论计算可得, 320 * 10 / 1500000 = 2.13ms, 实测结果和最理想的结果相差不大
 *
 *
 * ---------------波特率 2000000 条件下，个人 Ubuntu20.04 系统进行试验---------------
 * 实测程序 1 秒内就会卡住,接收不到应有的全部数据。自己在串口助手也会观察到几十帧正常数据就丢失几个字符的情况
 * 这不是偶然,而是每次都会发生。
 *
 */
void test() {
    while (1) {
        logger->debug("---------------test：收发字符测试---------------");

        // 发送和接收的字节数
        uint16_t test_size = 64;

        // 每次发送前，生成一个随机 id 号，用于和接收的字节比较
        uint8_t tx_data_id = GenerateRandomNumber();

        // 定义待发送字节
        uint8_t tx_data[256] = {
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b',
            'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd',
            'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b',
            'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd',
            'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b',
            'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

        // 待发送字节的第一位设置为 id 号
        tx_data[0] = tx_data_id;

        // 将字节发送到串口
        serialPort.write(reinterpret_cast<const char *>(tx_data), test_size);

        // 等待字节发送完成
        serialPort.DrainWriteBuffer();

        // 此时时间记为发送完成时间
        logger->debug("发送一帧字节，tx_data_id = {}", tx_data_id);

        // 定义接收字节
        uint8_t rx_data[256];
        for (uint16_t i = 0; i < test_size; i++) {
            char ch;
            serialPort.get(ch);
            rx_data[i] = ch;

            static uint64_t count = 0;
            count++;
            logger->trace("接收到字节：{}, count = {}", ch, count);
        }

        // 利用 id 号检验接收到的字节是否和发送的字节一致
        if (rx_data[0] != tx_data_id) {
            logger->error("接收到的字节和发送的字节不一致！");
            return;
        }

        static uint64_t count2 = 0;
        count2++;
        logger->debug("接收一帧字节，count2 = {}, rx_data[0] = {}", count2, rx_data[0]);  // 此时时间记为接收完成时间
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
    // serialPort.SetBaudRate(LibSerial::BaudRate::BAUD_921600);
    serialPort.SetBaudRate(LibSerial::BaudRate::BAUD_1500000);
    // serialPort.SetBaudRate(LibSerial::BaudRate::BAUD_2000000);
    if (!serialPort.good()) {
        spdlog::error("无法打开串口");
        return -1;
    }
    logger->info("串口打开成功！");

    test();

    return 0;
}