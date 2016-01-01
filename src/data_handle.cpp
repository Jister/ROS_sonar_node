#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include "ros/ros.h"
#include "sonar/Sonar.h"
#include "sonar/Sonar_raw.h"
#include "sonar/Laser.h"
#include "sensor_msgs/LaserScan.h"

int Front = 0;
int Back = 0;
int Left = 0;
int Right = 0;
int angle = 0;
int up = 0;
float min_distance = 0;

void scanCallback(const sensor_msgs::LaserScan laser)
{
  ROS_INFO("size:%d",laser.ranges.size());
  min_distance = laser.ranges[0];
  angle = 0;
  for(int i=0; i<laser.ranges.size(); i=i+4)
  {
    if(laser.ranges[i]<min_distance)
    {
      min_distance = laser.ranges[i];
      angle=i*0.25;
    }
  }
  min_distance = min_distance*100;
} 	

void sonarCallback(const sonar::Sonar_raw sonar)
{
    Right=sonar.sonar_1;
    Left =sonar.sonar_2;
    if(sonar.sonar_5<sonar.sonar_3)
    {
      Front=sonar.sonar_5;
    }else{
      Front=sonar.sonar_3;
    }
    if(sonar.sonar_4<sonar.sonar_6)
    {
      Back=sonar.sonar_4;
    }else{
      Back=sonar.sonar_6;
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "data_handle");
  ros::NodeHandle n;
  ros::Publisher sonar_pub = n.advertise<sonar::Sonar>("sonar_send",1000);
  ros::Publisher laser_pub = n.advertise<sonar::Laser>("laser_send",1000);
  ros::Subscriber sub1 = n.subscribe("sonar_data", 1000, sonarCallback);
  ros::Subscriber sub2 = n.subscribe("scan", 1000, scanCallback);
  ros::Rate loop_rate(25);

  while(ros::ok())
  {
    sonar::Sonar sonar_msg;
    sonar::Laser laser_msg;
    
    sonar_msg.sonar_front = 100.0;
   // sonar_msg.sonar_front = Front;
    sonar_msg.sonar_behind = Back;
    sonar_msg.sonar_left = Left;
    sonar_msg.sonar_right = Right;
   // laser_msg.min_distance = min_distance;
    laser_msg.min_distance = 200.0;
   // laser_msg.angle = angle;
    laser_msg.angle = 300.0;
    
    sonar_pub.publish(sonar_msg);
    laser_pub.publish(laser_msg);
      
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
