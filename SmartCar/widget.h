#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QTimer>
#include <QPixmap>
#include <QtNetwork>
#include <opencv2/opencv.hpp>
#include <QAbstractSocket>

using namespace cv;

namespace Ui {
class Widget;
}

class QTcpServer;
class QTcpSocket;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QImage MatToQImage(const Mat &mat);

    void camshiftalgorithm(Mat &image);

signals:
    void sendframedata();

public slots:
    void getrect(Rect);

    void getcompletionsign(int);

protected:


private slots:
    void opencamara();

    void showframe(QByteArray&);

    void acceptconnection();

    void getframe();

    void displayerror(QAbstractSocket::SocketError);

    void on_camshift_clicked();

private:
    Ui::Widget *ui;
    QTimer *timer;
    VideoCapture cam;
    Mat frame;
    QByteArray data;
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
    //qint64 bytesleft;
    //qint64 cache;
    //qint64 framesize;
    //qint64 receivedbytes;
    QByteArray secframebytes;
    //QByteArray secframehead;
    QImage image;
    Rect selection;
    Rect trackWindow;
    Mat hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
    bool selectObject;
    int trackObject;
};

#endif // WIDGET_H
