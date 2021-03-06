#include "worker.h"
#include <QDebug>

Worker::Worker(QObject *parent) : QObject(parent)
{

}

Worker::~Worker()
{
    delete tcpserver;
}

void Worker::netinit()
{
    qDebug()<<"start";
    tcpserver=new QTcpServer();
    if(!tcpserver->listen(QHostAddress::Any,6668))
    {
        qDebug()<<"listening in thread failture!!!";
        exit(1);
    }
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(acceptconnection()));
}

void Worker::acceptconnection()
{
    qDebug()<<"connection success!!!";
    tcpsocket=tcpserver->nextPendingConnection();
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(deepinfromrpi()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayerror(QAbstractSocket::SocketError)));
    tcpserver->close();
}

void Worker::displayerror(QAbstractSocket::SocketError)
{
    qDebug()<<tcpsocket->errorString()<<"thread";
    tcpserver->close();
}

void Worker::deepinfromrpi()
{
    rpidata.append(tcpsocket->readAll());
    secdata=rpidata.left(000);
    rpidata.remove(0,000);

    //emit rpidata(xxx);
}


void Worker::deepintorpi(const char *direction)
{
    char data=direction[0];
    if((tcpsocket->write((const char*)&data,sizeof(data)))==-1)
        qDebug()<<"write drivedata error";
}
