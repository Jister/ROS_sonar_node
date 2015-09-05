#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

#include "ros/ros.h"
#include "sonar/Sonar.h"

int sonar1_start_time;
int sonar1_stop_time;
int sonar1_time;
int sonar2_start_time;
int sonar2_stop_time;
int sonar2_time;
int sonar3_start_time;
int sonar3_stop_time;
int sonar3_time;
int sonar4_start_time;
int sonar4_stop_time;
int sonar4_time;
int sonar5_start_time;
int sonar5_stop_time;
int sonar5_time;
int sonar6_start_time;
int sonar6_stop_time;
int sonar6_time;

void Interrupt1()
{
  if(digitalRead(0) == 1)
  {
    sonar1_start_time = micros();
  }
  if(digitalRead(0) == 0)
  {
    sonar1_stop_time = micros();
    digitalWrite(21,LOW);
    digitalWrite(22,HIGH);
  }
}

void Interrupt2()
{
  if(digitalRead(1) == 1)
  {
    sonar2_start_time = micros();
  }
  if(digitalRead(1) == 0)
  {
    sonar2_stop_time = micros();
    digitalWrite(22,LOW);
    digitalWrite(23,HIGH);
  }
}

void Interrupt3()
{
  if(digitalRead(2) == 1)
  {
    sonar3_start_time = micros();
  }
  if(digitalRead(2) == 0)
  {
    sonar3_stop_time = micros();
    digitalWrite(23,LOW);
    digitalWrite(24,HIGH);
  }
}

void Interrupt4()
{
  if(digitalRead(3) == 1)
  {
    sonar4_start_time = micros();
  }
  if(digitalRead(3) == 0)
  {
    sonar4_stop_time = micros();
    digitalWrite(24,LOW);
    digitalWrite(25,HIGH);
  }
}

void Interrupt5()
{
  if(digitalRead(4) == 1)
  {
    sonar5_start_time = micros();
  }
  if(digitalRead(4) == 0)
  {
    sonar5_stop_time = micros();
    digitalWrite(25,LOW);
    digitalWrite(26,HIGH);
  }
}

void Interrupt6()
{
  if(digitalRead(5) == 1)
  {
    sonar6_start_time = micros();
  }
  if(digitalRead(5) == 0)
  {
    sonar6_stop_time = micros();
    digitalWrite(26,LOW);
    digitalWrite(21,HIGH);
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonar_node");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<sonar::Sonar>("sonar_data_raw", 1000);
  ros::Rate loop_rate(10);
  float distance1 = 0;
  float distance2 = 0;   
  float distance3 = 0;   
  float distance4 = 0;   
  float distance5 = 0;   
  float distance6 = 0;   
  wiringPiSetup();
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  digitalWrite(21, HIGH);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);

  wiringPiISR(0,INT_EDGE_BOTH,&Interrupt1);
  wiringPiISR(1,INT_EDGE_BOTH,&Interrupt2);
  wiringPiISR(2,INT_EDGE_BOTH,&Interrupt3);
  wiringPiISR(3,INT_EDGE_BOTH,&Interrupt4);
  wiringPiISR(4,INT_EDGE_BOTH,&Interrupt5);
  wiringPiISR(5,INT_EDGE_BOTH,&Interrupt6);

  while (ros::ok())
  {
    sonar::Sonar msg;
    
    if(sonar1_stop_time > sonar1_start_time)
    {
      sonar1_time = sonar1_stop_time - sonar1_start_time;
    }
    if(sonar2_stop_time > sonar2_start_time)
    {
      sonar2_time = sonar2_stop_time - sonar2_start_time;
    }
    if(sonar3_stop_time > sonar3_start_time)
    {
      sonar3_time = sonar3_stop_time - sonar3_start_time;
    }
    if(sonar4_stop_time > sonar4_start_time)
    {
      sonar4_time = sonar4_stop_time - sonar4_start_time;
    }
    if(sonar5_stop_time > sonar5_start_time)
    {
      sonar5_time = sonar5_stop_time - sonar5_start_time;
    }
    if(sonar6_stop_time > sonar6_start_time)
    {
      sonar6_time = sonar6_stop_time - sonar6_start_time;
    }
    distance1 = sonar1_time/58;
    distance2 = sonar2_time/58;
    distance3 = sonar3_time/58;
    distance4 = sonar4_time/58;
    distance5 = sonar5_time/58;
    distance6 = sonar6_time/58;
 
    msg.sonar_1 = distance1;
    msg.sonar_2 = distance2;
    msg.sonar_3 = distance3;
    msg.sonar_4 = distance4;
    msg.sonar_5 = distance5;
    msg.sonar_6 = distance6;

    ROS_INFO("%f", distance1);
    ROS_INFO("%f", distance2);
    ROS_INFO("%f", distance3);
    ROS_INFO("%f", distance4);
    ROS_INFO("%f", distance5);
    ROS_INFO("%f", distance6);
  
    chatter_pub.publish(msg);
  
    ros:: spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
