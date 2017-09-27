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


string getarduinodata(int &serial, string *buf)
{	
	char *tmpbuf=(char*)malloc(128);
    if(serialDataAvail(serial))
    {
    	int num=0;
    	if((num=read(serial,tmpbuf,serialDataAvail(serial)))!=-1)
    	{
    		buf->append(tmpbuf,num);
    		free(tmpbuf);
    		tmpbuf=NULL;
    		//strcat(buf,tmpbuf);
    		if(buf->size()>=8)
    		{
               if((*buf)[0]=='H')
               {
                   string tmpdata;
                   tmpdata=buf->substr(0,8);
                   buf->erase(0,8);
                   return tmpdata;
               }
               else
               {
               	   cout<<"data error!!!"<<endl;
               	   exit(1);
               }
    		}
    		else
    			return string();
    	}
    	else
    	{
    		cout<<"read data error!!!"<<endl;
    		exit(1);
    	}
    }
    else
    	return string();
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
