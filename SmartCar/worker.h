#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QtNetwork>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

signals:
    //void rpidata(xxx);

public slots:
    void netinit();

    void acceptconnection();

    void deepinfromrpi();

    void deepintorpi();

    void dirdata(QString, int);

    //void autodridata();

private:
    QTcpServer subtcpserver;
    QTcpSocket *tcpsocket;

};

#endif // WORKER_H
