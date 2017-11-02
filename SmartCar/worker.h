#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QtNetwork>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

    QTcpServer *subtcpserver;

signals:

public slots:
    void netinit();

    void acceptconnection();

    void deepinfromrpi();

    void deepintorpi();

    void dirdata(QString, int);

    //void autodridata();
};

#endif // WORKER_H
