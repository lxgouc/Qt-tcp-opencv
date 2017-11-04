#include "worker.h"
#include <QDebug>

Worker::Worker(QObject *parent) : QObject(parent)
{

}

void Worker::netinit()
{
    if(!subtcpserver.listen(QHostAddress::Any,6668))
    {
        qDebug()<<"listening in thread failture!!!";
        exit(1);
    }
    connect(&subtcpserver,SIGNAL(newConnection()),this,SLOT(acceptconnection()));
}

void Worker::acceptconnection()
{
    qDebug()<<"connection success!!!";
    tcpsocket=subtcpserver.nextPendingConnection();
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(deepinfromrpi()));
}

void Worker::deepinfromrpi()
{
    /*
     */
    //emit rpidata(xxx);
}

void Worker::deepintorpi()
{

}

void Worker::drivedata(QString direction, int value)
{

}
