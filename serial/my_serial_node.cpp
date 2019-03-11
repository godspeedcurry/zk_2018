#include "ros/ros.h"
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
serial::Serial ros_ser;
//回调函数
void callback(const std_msgs::String::ConstPtr& msg){
     ROS_INFO_STREAM("Write to serial port" << msg->data);
     ros_ser.write(msg->data);
 }
int main (int argc, char** argv){
     ros::init(argc, argv, "my_serial_node");
     ros::NodeHandle n;
     //订阅主题command
     ros::Subscriber command_sub = n.subscribe("command", 1000, callback);
     //发布主题sensor
     ros::Publisher sensor_pub = n.advertise<std_msgs::String>("sensor", 1000);
     try
     {
         ros_ser.setPort("/dev/ttyUSB0");
         ros_ser.setBaudrate(115200);
         serial::Timeout to = serial::Timeout::simpleTimeout(1000);
         ros_ser.setTimeout(to);
         ros_ser.open();
     }
     catch (serial::IOException& e)
     {
         ROS_ERROR_STREAM("Unable to open port ");
         return -1;
     }
     if(ros_ser.isOpen()){
         ROS_INFO_STREAM("Serial Port opened");
     }else{
         return -1;
     }
     ros::Rate loop_rate(10);
     while(ros::ok()){
         ros::spinOnce();
         if(ros_ser.available()){
             ROS_INFO_STREAM("Reading from serial port");
             std_msgs::String serial_data;
             //获取串口数据
             serial_data.data = ros_ser.read(ros_ser.available());
             ROS_INFO_STREAM("Read: " << serial_data.data);
             //将串口数据发布到主题sensor
             sensor_pub.publish(serial_data);
         }
         loop_rate.sleep();
     }
 }
