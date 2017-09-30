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
