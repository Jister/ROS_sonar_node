#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "ros/ros.h"
#include "sonar/Sonar_raw.h"

#define SERIAL_PORT "/dev/ttyAMA0"
#define MAXSIZE 60

sonar::Sonar_raw sonar_raw;

/*ringbuffer*/  
char ringbuf[MAXSIZE];
char readbuf[18];
unsigned short  data[9];
int read_addr = 0;  
int write_addr = 0;  
unsigned short crc_data= 0;

int next_data_handle(int addr)     
{     
  return (addr + 1) == MAXSIZE ?  0 : (addr + 1) ;     
}     

void write_data(char data)  
{  
  *(ringbuf+write_addr) = data;  
  write_addr = next_data_handle(write_addr);  
}  

/*crc*/
unsigned short crc_update(unsigned short  crc ,  unsigned char data)
{
  data ^= (crc & 0xff) ;
  data ^= data << 4;
  return ((((unsigned short)data << 8) | ((crc >> 8) & 0xff) )^ (unsigned char)(data >> 4) ^ ((unsigned short)data << 3)) ;
}

unsigned short crc(void* data, unsigned short count)
{
  unsigned short crc = 0xff;
  unsigned char *ptr = (unsigned char*)data;
  for(int i = 0 ; i < count ; i++)
  {
    crc = crc_update(crc , *ptr);
    ptr++;
  }
  return crc;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "sonar_node");
  ros::NodeHandle n;
  ros::Publisher sonar_pub = n.advertise<sonar::Sonar_raw>("sonar_data", 1000);
  ros::Rate loop_rate(25);

  int fd ;
  int ret ; 
/*open the serial*/
  if ((fd = serialOpen ( SERIAL_PORT, 57600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  while(ros::ok())
  {
    for(int i = 0 ; i < 29 ; i++)
    {
      if( ret = serialGetchar(fd) < 0 )
      {
        fprintf (stderr, "No data : %s\n", strerror (errno)) ;
        return 1;
      }
      ROS_INFO("%X",serialGetchar(fd));
    }

    ros:: spinOnce();
    loop_rate.sleep();
  }
  return 0;
}

