#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <QImage>
#include <QDataStream>
#include <QBuffer>
#include <opencv2/opencv.hpp>
#include "pthread_frame.h"

using namespace cv;
using namespace std;

int network_init_frame(char *str, uint16_t port)
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

void getframe(int tcpsocket)
{
	VideoCapture cap(0);
	if(!cap.isOpened())
	{
		perror("open camera failture!!!\n");
		exit(1);
	}
	Mat frame;
	vector<unsigned char>buff;
	vector<int>param;
	param.push_back(CV_IMWRITE_JPEG_QUALITY);
	param.push_back(95);
	while(true)
	{
        cap>>frame;
		if(frame.empty())
		{
			perror("frame is empty!!!\n");
			exit(1);
		}
		//cvtColor(frame,frame,CV_BGR2RGB);
		imencode(".jpg",frame,buff,param);
		vector<unsigned char>::size_type size=buff.size();
		sendframe(buff,size,tcpsocket);
		waitKey(33);        
	}
}

void sendframe(vector<unsigned char> &tmpframe, const vector<unsigned char>::size_type &size, const int &tcpsocket)
{
	int num;
    if((num=write(tcpsocket,&size,sizeof(size))<0))
    {
		perror("write error!!!");
		exit(1);
	}
	int val;
	if((val=write(tcpsocket,tmpframe.data(),tmpframe.size()))<0)
	{
		perror("write frame error!\n");
		exit(1);
	}	
}
