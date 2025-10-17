#include <chrono>
#include <random> 
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "assignment1_hajra/msg/robot_info.hpp"
using namespace std::chrono_literals;

class RobotInfoPublisher : public rclcpp::Node
{
public:
  RobotInfoPublisher()
  : Node("robot_info_publisher"), count_(0), distribution_(20.0, 40.0) 
  {
    publisher_ = this->create_publisher<assignment1_hajra::msg::RobotInfo>("robot_info", 10);
    auto timer_callback =
      [this]() -> void {
        auto message = assignment1_hajra::msg::RobotInfo();
        message.robot_name = "Robot 1";
        message.pose.x=5;
        message.pose.y=10;
        message.pose.theta=0.5; //radian
        message.temperature = distribution_(generator_); //generate random temperature value between 20 to 40
        RCLCPP_INFO(this->get_logger(), "Publishing: robot name=%s x=%.2f y=%.2f theta=%.2f temperature=%.2f",    message.robot_name.c_str(),message.pose.x,message.pose.y,message.pose.theta,message.temperature);
        this->publisher_->publish(message);
        count_++;
      };
    timer_ = this->create_wall_timer(1000ms, timer_callback);
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<assignment1_hajra::msg::RobotInfo>::SharedPtr publisher_;
  size_t count_;
  std::default_random_engine generator_;
  std::uniform_real_distribution<float> distribution_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RobotInfoPublisher>());
  rclcpp::shutdown();
  return 0;
}
