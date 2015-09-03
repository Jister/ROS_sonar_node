#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

#include "ros/ros.h"
#include "sonar/Sonar.h"

int sonar0_start_time;
int sonar0_stop_time;
int sonar0_time;
int sonar1_start_time;
int sonar1_stop_time;
int sonar1_time;
int sonar2_start_time;
int sonar2_stop_time;
int sonar2_time;

void Interrupt0()
{
  if(digitalRead(0) == 1)
  {
    sonar0_start_time = micros();
  }
  if(digitalRead(0) == 0)
  {
    sonar0_stop_time = micros();
    digitalWrite(21,LOW);
    digitalWrite(22,HIGH);
  }
}

void Interrupt1()
{
  if(digitalRead(1) == 1)
  {
    sonar1_start_time = micros();
  }
  if(digitalRead(1) == 0)
  {
    sonar1_stop_time = micros();
    digitalWrite(22,LOW);
    digitalWrite(23,HIGH);
  }
}

void Interrupt2()
{
  if(digitalRead(2) == 1)
  {
    sonar2_start_time = micros();
  }
  if(digitalRead(2) == 0)
  {
    sonar2_stop_time = micros();
    digitalWrite(23,LOW);
    digitalWrite(21,HIGH);
  }
}

void Filter(float sonar)
{
}
  
int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonar_node");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<sonar::Sonar>("sonar_data", 1000);
  ros::Rate loop_rate(10);
  float distance0 = 0;
  float distance1 = 0;   
  float distance2 = 0;   
  wiringPiSetup();
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  wiringPiISR(0,INT_EDGE_BOTH,&Interrupt0);
  wiringPiISR(1,INT_EDGE_BOTH,&Interrupt1);
  wiringPiISR(2,INT_EDGE_BOTH,&Interrupt2);

  digitalWrite(21, HIGH);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  while (ros::ok())
  {
    sonar::Sonar msg;
    
    if(sonar0_stop_time > sonar0_start_time)
    {
      sonar0_time = sonar0_stop_time - sonar0_start_time;
    }
    if(sonar1_stop_time > sonar1_start_time)
    {
      sonar1_time = sonar1_stop_time - sonar1_start_time;
    }
    if(sonar2_stop_time > sonar2_start_time)
    {
      sonar2_time = sonar2_stop_time - sonar2_start_time;
    }
    distance0 = sonar0_time/58;
    distance1 = sonar1_time/58;
    distance2 = sonar2_time/58;
    msg.sonar_1 = distance0;
    msg.sonar_2 = distance1;
    msg.sonar_3 = distance2;

    ROS_INFO("%f", distance0);
    ROS_INFO("%f", distance1);
    ROS_INFO("%f", distance2);
   
    chatter_pub.publish(msg);
  
    ros:: spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
