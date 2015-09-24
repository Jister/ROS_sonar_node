#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "ros/ros.h"
#include "sonar/Sonar.h"
#include "sensor_msgs/LaserScan.h"

int Front = 0;
int Back = 0;
int Left = 0;
int Right = 0;
int angle = 0;
float min_distance = 0;
char send_buf[22];
int fd;
int ret;

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

void sonarCallback(const sonar::Sonar sonar)
{
/*  if(sonar.sonar_1>30 && sonar.sonar_1<80){
    Right = 2;
  }else if(sonar.sonar_1>=80 && sonar.sonar_1<120){
    Right = 1;
  }else{
    Right = 0;
  }

  if(sonar.sonar_2>30 && sonar.sonar_2<80){
    Left = 2;
  }else if(sonar.sonar_2>=80 && sonar.sonar_2<120){
    Left = 1;
  }else{
    Left = 0;
  }

  if( (sonar.sonar_3>30 && sonar.sonar_3<80) ||(sonar.sonar_5>30 && sonar.sonar_5<80) ) {
    Front = 2;
  }else if((sonar.sonar_3>=80 && sonar.sonar_3<120)||(sonar.sonar_5>=80 && sonar.sonar_5<120)){
    Front = 1;
  }else{
    Front = 0;
  }

  if( (sonar.sonar_4>30 && sonar.sonar_4<80) ||(sonar.sonar_6>30 && sonar.sonar_6<80) ) {
    Back = 2;
  }else if((sonar.sonar_4>=80 && sonar.sonar_4<120)||(sonar.sonar_6>=80 && sonar.sonar_6<120)){
    Back = 1;
  }else{
    Back = 0;
  }*/
  
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
  
    sprintf(send_buf,"M%03d%05d%03d%03d%03d%03d\n" ,angle ,(int)min_distance, Front , Back , Left , Right);
    serialPuts(fd,send_buf);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonar_handle");
  ros::NodeHandle n;
  ros::Subscriber sub1 = n.subscribe("sonar_data", 1000, sonarCallback);
  ros::Subscriber sub2 = n.subscribe("scan", 1000, scanCallback);

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
