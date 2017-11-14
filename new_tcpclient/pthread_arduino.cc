#include "pthread_arduino.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "network_init.h"

using namespace std;
#define NUM 8
#define PORT1 6668
#define SIZE 1

bool rpifromarduino(int &serial, char *line)
{
  int len=0;
  if(serialDataAvail(serial))
  {
    while(len<NUM)
      len+=read(serial,line+len,NUM-len);
    return true;
  }
  else
    return false;
}

void rpitodeepin(int socket, const char* line)
{
    if((write(socket,line,NUM)==-1))
    {
    	perror("write data to deepin error!!1\n");
    	exit(1);
    }
}

void *rpifromdeepin(void *arg)
{
    int *fildes=(int*)arg;
    int len=0;
    char line[SIZE];
    while(1)
    {
      while(len<SIZE)
        len+=read(fildes[0],&line[len],SIZE-len);
      rpitoarduino(line,fildes[1]);
      len=0;
      bzero(line,SIZE);
    }
}

void rpitoarduino(char *line, const int &serial)
{
    
    static char tmp;
    if(tmp==line[0])
      serialPutchar(serial,line[0]);
    tmp=line[0];
}

void *pthread_xxx(void *arg)
{
  int tcpsocket=network_init((const char*)arg,PORT1);
  if(wiringPiSetup()<0)
  {
    perror("wiringPiSetup error!!!\n");
    exit(1);
  }
  int serial;
  if((serial=serialOpen("/dev/ttyACM0",19200))<0)
  {
    perror("can`t find arduino!!!\n");
    exit(1);
  }
  pthread_t rpithid;
  int fildes[2]={tcpsocket,serial};
  if((pthread_create(&rpithid,NULL,rpifromdeepin,(void*)fildes))!=0)
  {
    perror("create deepintorpi thread error!\n");
    exit(1);
  }
  char line[128];
  while(1)
  {  
    /*if(rpifromarduino(serial,line))
    {
      rpitodeepin(tcpsocket,line);
      bzero(line,128);
    }*/
  }
    pthread_exit(NULL);
}