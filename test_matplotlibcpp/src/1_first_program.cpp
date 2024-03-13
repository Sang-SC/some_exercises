#include "test_matplotlibcpp/matplotlibcpp.h"

#include <vector>
#include <cmath>

namespace plt = matplotlibcpp;

int main() {
    // 打开第一张图纸，并设置宽度和高度
    plt::figure_size(1600, 1200);

    //--------------------测试1，仅指定纵坐标，直接绘图，则横坐标默认为0, 1, 2, 3……--------------------
    plt::plot({0.0, 0.8, 1.6, 2.4});

    //--------------------测试2，指定横纵坐标，绘制sin函数--------------------
    std::vector<double> x2(1000);
    std::vector<double> y2(x2.size());

    for (size_t i = 0; i < x2.size(); i++) {
        x2[i] = i / 100.0;
        y2[i] = std::sin(2.0 * M_PI * x2[i]);
    }

    plt::plot(x2, y2);

    //--------------------测试3，测试不同颜色、线型--------------------
    std::vector<double> x3(10);
    std::vector<double> y31(x3.size());
    std::vector<double> y32(x3.size());
    std::vector<double> y33(x3.size());
    std::vector<double> y34(x3.size());
    std::vector<double> y35(x3.size());

    for (size_t i = 0; i < x3.size(); i++) {
        x3[i] = i;
        y31[i] = x3[i];
        y32[i] = x3[i] + 1;
        y33[i] = x3[i] + 2;
        y34[i] = x3[i] + 3;
        y35[i] = x3[i] + 4;
    }

    plt::plot(x3, y31, "b-");   // 蓝色实线
    plt::plot(x3, y32, "g--");  // 绿色虚线
    plt::plot(x3, y33, "r-.");  // 红色点划线
    plt::plot(x3, y34, "y:");   // 黄色点线
    plt::plot(x3, y35, "c.");   // 青色点（没有线了）

    //--------------------测试4，测试图例--------------------
    std::vector<double> x4(10);
    std::vector<double> y41(x4.size());
    std::vector<double> y42(x4.size());

    for (size_t i = 0; i < x4.size(); i++) {
        x4[i] = i;
        y41[i] = 10 - x4[i];
        y42[i] = 9 - x4[i];
    }

    plt::named_plot("y41", x4, y41, "b--");
    plt::named_plot("y42", x4, y42, "r--");

    // 显示右上角的图例
    plt::legend();

    // 设置有无网格
    plt::grid(true);

    // 设置标题、横纵坐标名称。不过中文无法显示
    plt::title("Test1, Test2, Test3, Test4");
    plt::xlabel("X");
    plt::ylabel("Y");

    // 设置坐标轴范围
    plt::xlim(0, 12);
    plt::ylim(-2, 12);

    // 保存第一章图纸内容为图片
    plt::save("figure1.png");

    //--------------------测试5，测试多张图纸以及子图--------------------
    std::vector<double> x5(10);
    std::vector<double> y51(x5.size());
    std::vector<double> y52(x5.size());

    for (size_t i = 0; i < x5.size(); i++) {
        x5[i] = i;
        y51[i] = x5[i];
        y52[i] = x5[i] * x5[i];
    }

    // 打开第二张图纸，并设置宽度和高度
    plt::figure_size(1200, 900);

    plt::subplot(2, 1, 1);
    plt::plot(x5, y51, "b-");

    plt::title("Test5 subplot(2, 1, 1)");
    plt::xlabel("X");
    plt::ylabel("Y");

    plt::subplot(2, 1, 2);
    plt::plot(x5, y52, "r-");

    plt::title("Test5 subplot(2, 1, 2)");
    plt::xlabel("X");
    plt::ylabel("Y");

    // 保存第二章图纸内容为图片
    plt::save("figure2.png");

    //--------------------显示绘图--------------------
    plt::show();
}