#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

#include "ros/ros.h"
#include "sonar/Sonar.h"

int sonar0_start_time;
int sonar0_stop_time;
int sonar0_time;
 
void Interrupt0()
{
  if(digitalRead(0) == 1)
  {
    sonar0_start_time = micros();
  }
  if(digitalRead(0) == 0)
  {
    sonar0_stop_time = micros();
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonar_node");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<sonar::Sonar>("sonar_data", 1000);
  ros::Rate loop_rate(100);
  int count = 0;
  float distance0 = 0;
  wiringPiSetup();
  wiringPiISR(0,INT_EDGE_BOTH,&Interrupt0);

  while (ros::ok())
  {
    sonar::Sonar msg;
    if(sonar0_stop_time > sonar0_start_time)
    {
      sonar0_time = sonar0_stop_time - sonar0_start_time;
    }
    distance0 = sonar0_time/58;
    msg.sonar_1 = distance0;

    ROS_INFO("%f", distance0);
   
    chatter_pub.publish(msg);
  
    ros:: spinOnce();
    loop_rate.sleep();
    ++count;
  }
  return 0;
}
