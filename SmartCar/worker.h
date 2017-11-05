#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QtNetwork>

struct Drivedata
{
    const char *direction=NULL;
    const char *speedval=NULL;
};

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

    void deepintorpi(const char*, const char*);

private:
    QTcpServer subtcpserver;
    QTcpSocket *tcpsocket;
    Drivedata drivevalue;
    QByteArray rpidata,secdata;

};

#endif // WORKER_H
