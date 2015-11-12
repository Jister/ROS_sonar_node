#include "ros/ros.h"
#include <math.h>
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
//#include <mavros_extras/Attitude.h>

#define bridge_distance  14

struct target{
  float distance;
  float angle;
  int count;
};

struct target range_1 = {0, 0, 0};
struct target range_2 = {0, 0, 0};
struct target range_3 = {0, 0, 0};
struct target range_4 = {0, 0, 0};

float x_1 = 0;
float y_1 = 0;
float x_2 = 0;
float y_2 = 0;
float x = 0;
float y = 0;

void scanCallback(const sensor_msgs::LaserScan msg)
{
  range_2.distance = 0;
  range_2.angle = 0;
  range_2.count = 0;
  range_1.distance = 0;
  range_1.angle = 0;
  range_1.count = 0;

  for(int i=msg.ranges.size()/2;i<msg.ranges.size()*5/6;i++)
  {
    if(!isnan(msg.ranges[i])){
        range_2.distance = range_2.distance + msg.ranges[i];
        range_2.angle = range_2.angle + i;
        range_2.count++;
      }
  }

  for(int i=msg.ranges.size()/6;i<msg.ranges.size()/2;i++)
  {
    if(!isnan(msg.ranges[i])){
        range_1.distance = range_1.distance + msg.ranges[i];
        range_1.angle = range_1.angle + i;
        range_1.count++;
      }
  }

  range_1.distance = range_1.distance/range_1.count;
  range_1.angle = range_1.angle/range_1.count*0.25 - 45;
  x_1 = range_1.distance*cos(range_1.angle/180*3.14);
  y_1 = range_1.distance*sin(range_1.angle/180*3.14);

  range_2.distance = range_2.distance/range_2.count;
  range_2.angle = range_2.angle/range_2.count*0.25 - 45;
  x_2 = range_2.distance*cos(range_2.angle/180*3.14);
  y_2 = range_2.distance*sin(range_2.angle/180*3.14);
  
  x = (bridge_distance - x_2 - x_1)/2;
  y = (y_1 + y_2)/2;

  ROS_INFO("x:%f",x);
  ROS_INFO("y:%f",y);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "laser_handle");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/scan_filtered", 1, scanCallback);
  ros::spin();

  return 0;
}
