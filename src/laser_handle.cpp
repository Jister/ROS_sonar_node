#include "ros/ros.h"
#include <math.h>
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/PoseStamped.h"

#define bridge_width  14
#define bridge_length  14
#define valid_value  4

struct target{
  float distance;
  float angle;
  int count;
};

struct target range_1 = {0, 0, 0};
struct target range_2 = {0, 0, 0};
struct target range_3 = {0, 0, 0};
struct target range_4 = {0, 0, 0};

float x_0 = 0;
float y_0 = 0;
float x_1 = 0;
float y_1 = 0;
float x_2 = 0;
float y_2 = 0;
float x_3 = 0;
float y_3 = 0;
float x_4 = 0;
float y_4 = 0;
float x = 0;
float y = 0;
float pose_x = 0;
float pose_y = 0;


void scanCallback(const sensor_msgs::LaserScan msg)
{
  range_2.distance = 0;
  range_2.angle = 0;
  range_2.count = 0;
  range_1.distance = 0;
  range_1.angle = 0;
  range_1.count = 0;
  range_3.distance = 0;
  range_3.angle = 0;
  range_3.count = 0;
  range_4.distance = 0;
  range_4.angle = 0;
  range_4.count = 0;

//second
  for(int i=msg.ranges.size()/2;i<msg.ranges.size()*5/6;i++)
  {
    if(!isnan(msg.ranges[i])){
        range_2.distance = range_2.distance + msg.ranges[i];
        range_2.angle = range_2.angle + i;
        range_2.count++;
      }
  }

//first
  for(int i=msg.ranges.size()/6;i<msg.ranges.size()/2;i++)
  {
    if(!isnan(msg.ranges[i])){
        range_1.distance = range_1.distance + msg.ranges[i];
        range_1.angle = range_1.angle + i;
        range_1.count++;
      }
  }

//third
  for(int i=msg.ranges.size()*5/6;i<msg.ranges.size();i++)
  {
    if(!isnan(msg.ranges[i])){
        range_3.distance = range_3.distance + msg.ranges[i];
        range_3.angle = range_3.angle + i;
        range_3.count++;
      }
  }

//forth
  for(int i=msg.ranges.size();i<msg.ranges.size()/6;i++)
  {
    if(!isnan(msg.ranges[i])){
        range_4.distance = range_4.distance + msg.ranges[i];
        range_4.angle = range_4.angle + i;
        range_4.count++;
      }
  }

  if(range_1.count>=valid_value){
    range_1.distance = range_1.distance/range_1.count;
    range_1.angle = range_1.angle/range_1.count*0.25 - 45;
    x_1 = range_1.distance*cos(range_1.angle/180*3.14);
    y_1 = range_1.distance*sin(range_1.angle/180*3.14);
  }else{
    x_1 = 0;
    y_1 = 0;
  }

  if(range_2.count>=valid_value){
    range_2.distance = range_2.distance/range_2.count;
    range_2.angle = range_2.angle/range_2.count*0.25 - 45;
    x_2 = range_2.distance*cos(range_2.angle/180*3.14);
    y_2 = range_2.distance*sin(range_2.angle/180*3.14);
  }else{
    x_2 = 0;
    y_2 = 0;
  }

  if(range_3.count>=valid_value){
    range_3.distance = range_3.distance/range_3.count;
    range_3.angle = range_3.angle/range_3.count*0.25 - 45;
    x_3 = range_3.distance*cos(range_2.angle/180*3.14);
    y_3 = range_3.distance*sin(range_2.angle/180*3.14);
  }else{
    x_3 = 0;
    y_3 = 0;
  }

  if(range_4.count>=valid_value){
    range_4.distance = range_4.distance/range_4.count;
    range_4.angle = range_4.angle/range_4.count*0.25 - 45;
    x_4 = range_4.distance*cos(range_4.angle/180*3.14);
    y_4 = range_4.distance*sin(range_4.angle/180*3.14);
  }else{
    x_4 = 0;
    y_4 = 0;
  }

  if(range_1.count>=valid_value && range_2.count>=valid_value){
    x = (bridge_width - x_2 - x_1)/2;
    y = (y_1 + y_2)/2;
  }else if(range_1.count>=valid_value && range_2.count<valid_value){
    x = bridge_width- x_1;
    y = y_1;
  }else if(range_1.count<valid_value && range_2.count>=valid_value){
    x = -x_2;
    y = y_2;
  }

  if(x_0 == 0){
    x_0 = x;
  }
  if(y_0 == 0){
    y_0 = y;
  }
  
  pose_x = x - x_0;
  pose_y = y - y_0;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "laser_handle");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/scan_filtered", 1, scanCallback);
  ros::Publisher pub = n.advertise<geometry_msgs::PoseStamped>("mavros/vision_pose/pose", 1000);
  ros::Rate loop_rate(50);

  while(ros::ok())
  {
    geometry_msgs::PoseStamped vicon;
    vicon.header.stamp = ros::Time::now();
    vicon.pose.position.x = -pose_y;
    vicon.pose.position.y = pose_x;
    pub.publish(vicon);
    ROS_INFO("position:%f-%f-%f", vicon.pose.position.x,vicon.pose.position.y,vicon.pose.position.z);
      
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
