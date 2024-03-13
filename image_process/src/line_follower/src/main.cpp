#include "ros/ros.h"
#include "line_follower/webotsInterface.hpp"

using namespace std;

ros::NodeHandle *n;

int main(int argc, char **argv) {
    setlocale(LC_CTYPE,"zh_CN.utf8");   // 控制台设置输出中文，否则就是乱码
    ros::init(argc, argv, "main");
    n = new ros::NodeHandle;

    webotsInit();  // 使能电机和摄像头
    pid_init(0.5,0.2,0.05);  // 自己一开始怎么执行都有问题，后来发现是忘了初始化 PID 参数……
    // enableKeyboard();  // 想用键盘控制时使用s

    while(ros::ok())
    {
        ros::spinOnce();
    }

}