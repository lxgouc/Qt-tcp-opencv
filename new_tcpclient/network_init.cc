#include <network_init.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

int network_init(const char *str, uint16_t port)
{
	struct sockaddr_in frameclient;
	int tcpsocket;
	if((tcpsocket=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("creat socket error!!!\n");
		exit(1);
	}
	else
		cout<<"creat socket success!!!"<<endl;
	bzero(&frameclient,sizeof(frameclient));
	frameclient.sin_family=AF_INET;
	frameclient.sin_port=htons(port);
	frameclient.sin_addr.s_addr=inet_addr(str);
	if(connect(tcpsocket,(struct sockaddr*)(&frameclient),sizeof(frameclient))<0)
	{
		perror("connect error!!!\n");
		exit(1);
	}	
	else
	{
		printf("connect success!!!\n");
		return tcpsocket;
    }
}