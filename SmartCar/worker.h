#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QtNetwork>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);
    ~Worker();

signals:
    //void rpidata(xxx);

public slots:
    void netinit();

    void acceptconnection();

    void deepinfromrpi();

    void deepintorpi(const char*);

    void displayerror(QAbstractSocket::SocketError);

private:
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
    QByteArray rpidata,secdata;

};

#endif // WORKER_H
