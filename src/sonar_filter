#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ros/ros.h"
#include "sonar/Sonar.h"

sonar::Sonar sonar_raw;
sonar::Sonar sonar_prev;
sonar::Sonar sonar_pprev;
sonar::Sonar sonar_flitered;

void SonarCallback(const sonar::Sonar sonar_msg)
{
  sonar_pprev = sonar_prev;
  sonar_prev = sonar_raw;
  sonar_raw = sonar_msg;
  if(sonar_raw.sonar_1 > 700){
    sonar_raw.sonar_1

  sonar_filtered.sonar_1 = (sonar_raw.sonar_1 + sonar_prev.sonar_1 + sonar_pprev.sonar_1)/3;
  sonar_filtered.sonar_2 = (sonar_raw.sonar_2 + sonar_prev.sonar_2 + sonar_pprev.sonar_2)/3;
  sonar_filtered.sonar_3 = (sonar_raw.sonar_3 + sonar_prev.sonar_3 + sonar_pprev.sonar_3)/3;
  sonar_filtered.sonar_4 = (sonar_raw.sonar_4 + sonar_prev.sonar_4 + sonar_pprev.sonar_4)/3;
  sonar_filtered.sonar_5 = (sonar_raw.sonar_5 + sonar_prev.sonar_5 + sonar_pprev.sonar_5)/3;
  sonar_filtered.sonar_6 = (sonar_raw.sonar_6 + sonar_prev.sonar_6 + sonar_pprev.sonar_6)/3;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonar_filter");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("sonar_data", 1000, SonarCallback);
  ros::spin();

  return 0;
}
