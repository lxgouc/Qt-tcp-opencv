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
#include "pthread_xxx.h"

using namespace std;
#define NUM 8
#define PORT1 6668

bool arduinotorpi(int &serial, char *line)
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
    	perror("write error!!1\n");
    	exit(1);
    }
}

void *deepintorpi(void *arg)
{

}

void rpitoarduino()
{

}

void *pthread_xxx(void *arg)
{
  int tcpsocket=network_init((char*)arg,PORT1);
  if(wiringPiSetup()<0)
  {
    perror("wiringPiSetup error!!!\n");
    exit(1);
  }
  int serial;
  if((serial=serialOpen("/dev/ttyACM0",9600))<0)
  {
    perror("can`t find arduino!!!\n");
    exit(1);
  }
  pthread_t rpithid;
  if((pthread_create(&rpithid,NULL,deepintorpi,(void*)(&tcpsocket)))!=0)
  {
    perror("create deepintorpi thread error!\n");
    exit(1);
  }
  while(1)
  {
    char *line=(char*)malloc(128);
    if(arduinotorpi(serial,line))
    {
      rpitodeepin(tcpsocket,line);
    }
    free(line);
  }
    pthread_exit(NULL);
}