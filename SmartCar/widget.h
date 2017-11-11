#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
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

    void detectAndDraw( Mat& img, CascadeClassifier& cascade,CascadeClassifier& nestedCascade,double scale, bool tryflip );

signals:
    void drivesignal(const char*, const char*);

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

    void on_facedetect_clicked();

    void threadinit();

    void on_UpButton_clicked();

    void on_LeftButton_clicked();

    void on_RightButton_clicked();

    void on_DownButton_clicked();

    void on_AutoDrive_clicked();

    void poscal(RotatedRect&);

    char errorvalue(const int&);

    //void displayrpidata(xxx);

private:
    Ui::Widget *ui;
    QTcpServer tcpserver;
    QTcpSocket *tcpsocket;
    QThread workthread;
    //qint64 bytesleft;
    //qint64 cache;
    //qint64 framesize;
    //qint64 receivedbytes;
    QByteArray secframebytes;
    //QByteArray secframehead;
    Rect selection;
    Rect trackWindow;
    Mat hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
    bool selectObject;
    bool autotrack;
    char selthm;
    int trackObject;
    CascadeClassifier cascade, nestedCascade;
};

#endif // WIDGET_H
