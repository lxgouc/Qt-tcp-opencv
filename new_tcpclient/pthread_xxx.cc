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

int network_init_xxx(char *str, uint16_t port)
{
	struct sockaddr_in xxxclient;
	int tcpsocket;
	if((tcpsocket=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("create socket error!!!\n");
		exit(1);
	}
	else
		cout<<"create socket success!!!"<<endl;
	bzero(&xxxclient,sizeof(xxxclient));
	xxxclient.sin_family=AF_INET;
	xxxclient.sin_port=htons(port);
	xxxclient.sin_addr.s_addr=inet_addr(str);
	if(connect(tcpsocket,(struct sockaddr*)(&xxxclient),sizeof(xxxclient))<0)
	{
		perror("connect error!!!\n");
		exit(1);
	}
	else
	{
		cout<<"connect success!!!"<<endl;
		return tcpsocket;
	}
}

bool getarduinodata(int &serial, char *line)
{
  int len=0;
  if(serialDataAvail(serial))
  {
    while(len<8)
      len+=read(serial,line+len,8-len);
    return true;
  }
  else
    return false;
}

void sendarduinodata(int socket, string line)
{
    const char *cstr=line.c_str();
    if((write(socket,cstr,line.size()))==-1)
    {
    	perror("write error!!1\n");
    	exit(1);
    }
}

/*char getraspberrydata(int socket)
{   
	char data[128];
	int num;
	if((num=read(socket,data,128))!=-1)
	{
		switch (data[0])
		{
			case '':

			case '':

			case '':

			case '':

            default:
		}
	}
}*/

void sendraspberrydata()
{

}
