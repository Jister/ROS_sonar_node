#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "ros/ros.h"
#include "sonar/Sonar.h"
#include "sensor_msgs/LaserScan.h"

int Back = 0;
int Left = 0;
int Right = 0;
int angle = 0;
int min_distance = 0;
char send_buf[19];
int fd;
int ret;

void scanCallback(const sensor_msgs::LaserScan laser)
{
  min_distance = laser.ranges[0];
  angle = 0;
  for(int i=0; i<laser.ranges.size(); i++)
  {
    if(laser.ranges[i]<min_distance)
    {
      min_distance = laser.ranges[i];
      angle=i*0.25;
    }
  }
  ROS_INFO("angle:%d",angle);
  ROS_INFO("distance:%d",min_distance);
} 	

void sonarCallback(const sonar::Sonar sonar)
{
    Right=sonar.sonar_1;
    Left =sonar.sonar_2;
    Back =sonar.sonar_3;
    sprintf(send_buf,"M%03d%05d%03d%03d%03d\n" ,angle , min_distance, Back , Left , Right);
    serialPuts(fd,send_buf);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonar_handle_2");
  ros::NodeHandle n;
  ros::Subscriber sub1 = n.subscribe("sonar_data", 1000, sonarCallback);
  ros::Subscriber sub2 = n.subscribe("scan", 5000, scanCallback);

  if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }
  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }

  ros::spin();
  
  return 0;
}
