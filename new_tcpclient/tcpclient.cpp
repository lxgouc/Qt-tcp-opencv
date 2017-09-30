#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "pthread_frame.h"
#include "pthread_xxx.h"

using namespace std;

int main(int argc,  char *argv[])
{
	if(argc!=2)
	{
		perror("argument error\n");
		exit(1);
	}
	pthread_t thid1, thid2;
	pthread_attr_t attr;
	cout<<"the main thread id:"<<pthread_self()<<endl;
    if(pthread_attr_init(&attr)!=0)
    {
		perror("attrbute init error!!!\n");
		exit(1);
	}
	if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)!=0)
	{
		perror("setting detached attrbute error!!!\n");
		exit(1);
	}
	if(pthread_create(&thid1,&attr,pthread_frame,(void*)argv[1])!=0)
	{
		perror("create thread_frame error!!!\n");
		exit(1);
	}
	cout<<"the thread_frame thread id:"<<thid1<<endl;
	if(pthread_create(&thid2,&attr,pthread_xxx,(void*)argv[1])!=0)
	{
		perror("create thread_xxx error!!!\n");
		exit(1);
	}
    cout<<"the thread_xxx thread id:"<<thid2<<endl;
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}



