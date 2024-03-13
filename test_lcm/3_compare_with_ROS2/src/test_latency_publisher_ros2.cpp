#include <rclcpp/rclcpp.hpp>                // ROS2节点的主头文件
#include <builtin_interfaces/msg/time.hpp>  // ROS2内置消息Time的头文件
#include "utils/logger_config.h"            // 自定义日志库

#include <chrono>

class PublisherNode : public rclcpp::Node {
   public:
    // 构造函数
    PublisherNode(std::string name) : Node(name) {
        logger->info("Node name: {}", name);
        publisher_ = this->create_publisher<builtin_interfaces::msg::Time>("test_latency", 10);
        timer_ = this->create_wall_timer(std::chrono::microseconds(1000), std::bind(&PublisherNode::TimerCallback, this));
    }

   private:
    // 定义发布者和定时器
    rclcpp::Publisher<builtin_interfaces::msg::Time>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    // 定义时间变量
    std::chrono::high_resolution_clock::time_point time_current_ = std::chrono::high_resolution_clock::now();

    // 定义定时器回调函数
    void TimerCallback() {
        // 获取当前时间
        time_current_ = std::chrono::high_resolution_clock::now();

        // 发布时间消息
        builtin_interfaces::msg::Time msg1;
        msg1.sec = std::chrono::duration_cast<std::chrono::seconds>(time_current_.time_since_epoch()).count();
        msg1.nanosec = std::chrono::duration_cast<std::chrono::nanoseconds>(time_current_.time_since_epoch() % std::chrono::seconds(1)).count();
        publisher_->publish(msg1);

        // 打印日志
        logger->info("ROS2: Publish successfully!");
    }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PublisherNode>("PublisherNode");
    auto executor = std::make_shared<rclcpp::executors::SingleThreadedExecutor>();
    executor->add_node(node);
    executor->spin();
    return 0;
}