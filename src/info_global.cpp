#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "assignment1_hajra/msg/robot_info.hpp"
class RobotInfoSubscriber : public rclcpp::Node
{
public:
  RobotInfoSubscriber()
  : Node("robot_info_subscriber")
  {
  publisher_ = this->create_publisher<assignment1_hajra::msg::RobotInfo>("robot_info_global", 10);
    auto topic_callback =
      [this](assignment1_hajra::msg::RobotInfo::UniquePtr msg) -> void {
         // Convert degree to radians
    double rotation = 50.0 * M_PI / 180.0;

    // Rotate local position into global frame
    double x_global = msg->pose.x * cos(rotation) - msg->pose.y * sin(rotation);
    double y_global = msg->pose.x * sin(rotation) + msg->pose.y * cos(rotation);
     // Adjust orientation
    double theta_global = msg->pose.theta + rotation;
    // Create a new message to publish in global frame
        auto global_msg = assignment1_hajra::msg::RobotInfo();
        global_msg.robot_name = msg->robot_name;
        global_msg.pose.x = x_global;
        global_msg.pose.y = y_global;
        global_msg.pose.theta = theta_global;
        global_msg.temperature = msg->temperature;
    

        RCLCPP_INFO(this->get_logger(),
      "Robot Name=%s Robot position in global frame: x=%.2f, y=%.2f, θ=%.2f and Temperature=%.2f",
      msg->robot_name.c_str(),x_global, y_global, theta_global,
      msg->temperature);
      
      this->publisher_->publish(global_msg);
      };
    subscription_ =
      this->create_subscription<assignment1_hajra::msg::RobotInfo>("robot_info", 10, topic_callback);
  }

private:
  rclcpp::Subscription<assignment1_hajra::msg::RobotInfo>::SharedPtr subscription_;
   rclcpp::Publisher<assignment1_hajra::msg::RobotInfo>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RobotInfoSubscriber>());
  rclcpp::shutdown();
  return 0;
}
