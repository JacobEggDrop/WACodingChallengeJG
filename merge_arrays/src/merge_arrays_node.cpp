#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/multi_array_layout.hpp"
#include "std_msgs/msg/int32_multi_array.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class MergeArrays : public rclcpp::Node
{
  

  public:
  int32_t *array1;
  int32_t *array2;
    MergeArrays()
    : Node("merge_arrays_node")
    {
      publisher_ = this->create_publisher<std_msgs::msg::Int32MultiArray>("/output/array", 10);

       subscriber1_ = this->create_subscription<std_msgs::msg::Int32MultiArray>(
            "/input/array1",
            10,  // Queue size
            std::bind(&MergeArrays::topicCallback1, this, std::placeholders::_1));
       subscriber2_ = this->create_subscription<std_msgs::msg::Int32MultiArray>(
            "/input/array2",
            10,  // Queue size
            std::bind(&MergeArrays::topicCallback2, this, std::placeholders::_1));

    }

  private:

    void topicCallback1(const std_msgs::msg::Int32MultiArray::SharedPtr msg) {

      int size = sizeof(msg->data);

      array1 = new int32_t[size];

        // Handle the received Int32MultiArray data here
        // msg->data is a vector<int32_t> containing the array elements

        for(int i = 0;i<size;i++) {
          array1[i] = msg->data[i];
        
        }

      publishMethod();

  
    }
    void topicCallback2(const std_msgs::msg::Int32MultiArray::SharedPtr msg) {

      int size = sizeof(msg->data);
      array2 = new int32_t[size];
        // Handle the received Int32MultiArray data here
        // msg->data is a vector<int32_t> containing the array elements

        for(int i = 0;i<size;i++) {
          array2[i] = msg->data[i];
        }

    }
    void publishMethod() {

      std_msgs::msg::Int32MultiArray array_msg;

      int size = 0;


      for (int i = 0; i < sizeof(array1)/sizeof(array1[0]); i++) {
        array_msg.data[i] = array1[i];
      }
      for (int j = 0; j < sizeof(array2)/sizeof(array2[0]); j++) {
        array_msg.data[sizeof(array2)/sizeof(array2[0]) + j] = array2[j];
      }

      

      for (int i = 0; i < sizeof(array_msg.data)/sizeof(array_msg.data[0])-1; i++) {
        for (int j = 0; j < sizeof(array_msg.data)/sizeof(array_msg.data[0])-i-1; j++) {
            if (array_msg.data[j] > array_msg.data[j+1]) {
                int temp = array_msg.data[j];
                array_msg.data[j] = array_msg.data[j+1];
                array_msg.data[j+1] = temp;
            }
        }
      }
   
      publisher_->publish(array_msg);


    }

    
    
  
    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr publisher_;
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr subscriber1_;
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr subscriber2_;

    

};
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MergeArrays>());
  rclcpp::shutdown();
  return 0;
}

