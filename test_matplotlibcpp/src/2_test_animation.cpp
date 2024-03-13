#include "test_matplotlibcpp/matplotlibcpp.h"

#include <vector>
#include <cmath>
#include <csignal>

/**
 * @brief 定义信号处理函数，以便在按下 Ctrl+C 时退出程序
 *
 * @param signal
 */
void signalHandler(int signal) {
    if (signal == SIGINT) {
        exit(-1);
    }
}

namespace plt = matplotlibcpp;

int main() {
    // 注册信号处理函数
    std::signal(SIGINT, signalHandler);

    // 打开第一张图纸，并设置宽度和高度
    plt::figure_size(1600, 1200);

    // 存储要绘制的数据
    std::vector<double> t;
    std::vector<double> y;

    // 生成数据
    double dt = 0.01;
    uint64_t num = 100;

    for (size_t i = 0; i < num; i++) {
        t.push_back(i * dt);
        y.push_back(std::sin(2.0 * M_PI * t[i]));

        plt::clf();                // 清除当前图形
        plt::plot(t, y, "r-");     // 绘制当前帧
        plt::xlim(0.0, num * dt);  // 设置 x 轴范围
        plt::ylim(-1.2, 1.2);      // 设置 y 轴范围
        plt::draw();               // 显示绘图
        plt::pause(dt);            // 暂停一段时间
    }

    // 关闭图形窗口
    plt::close();

    return 0;
}