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

sonar::Sonar sonar_raw;      //当前声纳数据      
sonar::Sonar sonar_prev;     //前一次声纳数据
sonar::Sonar sonar_pprev;    //上上次的声纳数据
sonar::Sonar sonar_filtered; //求平均后声纳数据


void Interrupt1()             //声纳1中断
{
  if(digitalRead(0) == 1)
  {
    sonar1_start_time = micros();    //上升沿触发时，记下时间
  }
  if(digitalRead(0) == 0)            //下降沿触发时，记下时间
  {
    sonar1_stop_time = micros();
    digitalWrite(21,LOW);            //关闭声纳1,开启声纳2
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
  ros::Publisher chatter_pub = n.advertise<sonar::Sonar>("sonar_data", 1000);
  ros::Rate loop_rate(20);
  float distance1 = 0;
  float distance2 = 0;   
  float distance3 = 0;   
  float distance4 = 0;   
  float distance5 = 0;   
  float distance6 = 0;   
  wiringPiSetup();
  pinMode(21, OUTPUT);               //设置引脚为输出模式
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  digitalWrite(21, HIGH);            //开启声纳1
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);

  wiringPiISR(0,INT_EDGE_BOTH,&Interrupt1);    //设置中断，为上升沿下降沿都触发
  wiringPiISR(1,INT_EDGE_BOTH,&Interrupt2);
  wiringPiISR(2,INT_EDGE_BOTH,&Interrupt3);
  wiringPiISR(3,INT_EDGE_BOTH,&Interrupt4);
  wiringPiISR(4,INT_EDGE_BOTH,&Interrupt5);
  wiringPiISR(5,INT_EDGE_BOTH,&Interrupt6);

  while (ros::ok())
  {
    if(sonar1_stop_time > sonar1_start_time)    //计算声纳高电平时间
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
    distance1 = sonar1_time/58;                  //计算声纳距离
    distance2 = sonar2_time/58;
    distance3 = sonar3_time/58;
    distance4 = sonar4_time/58;
    distance5 = sonar5_time/58;
    distance6 = sonar6_time/58;
 
    sonar_pprev = sonar_prev;
    sonar_prev = sonar_raw;
    sonar_raw.sonar_1 = distance1;
    sonar_raw.sonar_2 = distance2;
    sonar_raw.sonar_3 = distance3;
    sonar_raw.sonar_4 = distance4;
    sonar_raw.sonar_5 = distance5;
    sonar_raw.sonar_6 = distance6;

    sonar_filtered.sonar_1 = (sonar_raw.sonar_1 + sonar_prev.sonar_1 + sonar_pprev.sonar_1)/3;    //求平均
    sonar_filtered.sonar_2 = (sonar_raw.sonar_2 + sonar_prev.sonar_2 + sonar_pprev.sonar_2)/3;
    sonar_filtered.sonar_3 = (sonar_raw.sonar_3 + sonar_prev.sonar_3 + sonar_pprev.sonar_3)/3;
    sonar_filtered.sonar_4 = (sonar_raw.sonar_4 + sonar_prev.sonar_4 + sonar_pprev.sonar_4)/3;
    sonar_filtered.sonar_5 = (sonar_raw.sonar_5 + sonar_prev.sonar_5 + sonar_pprev.sonar_5)/3;
    sonar_filtered.sonar_6 = (sonar_raw.sonar_6 + sonar_prev.sonar_6 + sonar_pprev.sonar_6)/3;

    chatter_pub.publish(sonar_filtered);
  
    ros:: spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
