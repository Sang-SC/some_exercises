#include <rclcpp/rclcpp.hpp>                // ROS2节点的主头文件
#include <builtin_interfaces/msg/time.hpp>  // ROS2内置消息Time的头文件
#include "utils/logger_config.h"            // 自定义日志库

#include <chrono>

class SubscriberNode : public rclcpp::Node {
   public:
    // 构造函数
    SubscriberNode(std::string name) : Node(name) {
        logger->info("节点名为：{}", name);
        subscriber_ = this->create_subscription<builtin_interfaces::msg::Time>(
            "test_latency", 10, std::bind(&SubscriberNode::SubscriberCallback, this, std::placeholders::_1));
    }

   private:
    // 定义发布者和定时器
    rclcpp::Subscription<builtin_interfaces::msg::Time>::SharedPtr subscriber_;

    // 订阅者回调函数
    void SubscriberCallback(const builtin_interfaces::msg::Time::SharedPtr msg) {
        auto time_current = std::chrono::high_resolution_clock::now();
        auto time_current_sec = std::chrono::duration_cast<std::chrono::seconds>(time_current.time_since_epoch()).count();
        auto time_current_nanosec =
            std::chrono::duration_cast<std::chrono::nanoseconds>(time_current.time_since_epoch() % std::chrono::seconds(1)).count();

        double time_delay_us = ((time_current_sec - msg->sec) * 1e9 + (time_current_nanosec - msg->nanosec)) / 1e3;

        // logger->info("  sec     = {}", msg->sec);
        // logger->info("  nanosec = {}", msg->nanosec);
        logger->info("ROS2: Subscribe successfully! Delay   = {} us", time_delay_us);
    }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SubscriberNode>("SubscriberNode");
    auto executor = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    executor->add_node(node);
    executor->spin();
    return 0;
}