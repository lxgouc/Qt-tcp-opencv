#include "worker.h"
#include <QDebug>

Worker::Worker(QObject *parent) : QObject(parent)
{

}

Worker::~Worker()
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
    rpidata.append(tcpsocket->readAll());
    secdata=rpidata.left(000);
    rpidata.remove(0,000);

    //emit rpidata(xxx);
}


void Worker::deepintorpi(const char *direction, const char *speedval)
{
    drivevalue.direction=direction;
    drivevalue.speedval=speedval;
    if((tcpsocket->write((const char*)(&drivevalue),sizeof(struct Drivedata)))==-1)
        qDebug()<<"write drivedata error";
}
