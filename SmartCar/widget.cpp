#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QDateTime>
#include <QMouseEvent>
#include <QPainter>
#include <vector>
#include <string.h>

using namespace std;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    tcpserver=new QTcpServer(this);
    selectObject=false;
    selthm='o';
    trackObject=0;
    //receivedbytes=0;
    //framesize=0;
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(acceptconnection()));
    connect(ui->SwitchButton,SIGNAL(clicked()),this,SLOT(opencamara()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::opencamara()
{
    if(ui->SwitchButton->text()=="ON")
    {
        if(!tcpserver->listen(QHostAddress::Any,6666))
        {
            qDebug()<<"listening failuture!!!!";
            exit(1);
        }
       ui->SwitchButton->setText("OFF");
    }
    else
    {
        if(tcpserver->isListening())
            tcpserver->close();
        else
            tcpsocket->disconnectFromHost();
        secframebytes.resize(0);
        //framesize=0;
        ui->SwitchButton->setText("ON");
        ui->Video->clear();
    }
}

void Widget::acceptconnection()
{
    tcpsocket=tcpserver->nextPendingConnection();
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(getframe()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayerror(QAbstractSocket::SocketError)));
    tcpserver->close();

}

void Widget::displayerror(QAbstractSocket::SocketError)
{
    qDebug()<<tcpsocket->errorString();
    tcpsocket->close();
    ui->Video->clear();
}

/*void Widget::showframe(QByteArray &frameval)
{
    //QDataStream tmp(&secframebytes,QIODevice::ReadWrite);
    QString strframe=QString::fromLatin1(frameval.data(),frameval.size());
    QByteArray finalframe=QByteArray::fromBase64(strframe.toLatin1());
    //finalframe=qUncompress(finalframe);
    image.loadFromData(finalframe);
    ui->Video->setPixmap(QPixmap::fromImage(image));
    //frameval.resize(0);
}

void Widget::getframe()
{
    secframebytes.append(tcpsocket->readAll());
    if((unsigned)secframebytes.size()>sizeof(qint64))
    {
        QDataStream indata(&secframebytes,QIODevice::ReadOnly);
        indata.setVersion(QDataStream::Qt_4_8);
        qint64 framesize;
        indata>>framesize;
        if(framesize+sizeof(qint64)<=(unsigned)secframebytes.size())
        {
            indata.device()->seek(sizeof(qint64));
            QByteArray data;
            indata>>data;
            data=data.mid(0,framesize);
            secframebytes.remove(0,framesize+sizeof(qint64));
            showframe(data);
        }
        else
            return;
    }
    else
        return;
}*/

void Widget::getframe()
{
    static bool hashead=true;
    secframebytes.append(tcpsocket->readAll());
    qDebug()<<secframebytes.size();
    static vector<unsigned char>::size_type num;
    if(hashead)
    {
        QByteArray head(secframebytes.left(sizeof(vector<unsigned char>::size_type)));
        secframebytes.remove(0,sizeof(vector<unsigned char>::size_type));
        memcpy(&num,head.data(),sizeof(vector<unsigned char>::size_type));
        hashead=false;
    }

    if(num<=(unsigned)secframebytes.size())
    {
        QByteArray data(secframebytes.mid(0,num));
        secframebytes.remove(0,num);
        hashead=true;
        num=0;
        showframe(data);
    }
    else
        return;
}

void Widget::showframe(QByteArray &frameval)
{
    unsigned char *strframe=(unsigned char*)(frameval.data());
    vector<unsigned char>::size_type strsize=frameval.size();
    vector<unsigned char>buffer(strframe,strframe+strsize);
    Mat frame=imdecode(buffer,CV_LOAD_IMAGE_COLOR);
    switch (selthm)
    {
    case 'c':
        camshiftalgorithm(frame);
        break;
    default:
        break;
    }
    //QImage image=MatToQImage(frame);
    cvtColor(frame,frame,CV_BGR2RGB);
    QImage image((const uchar*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888);
    ui->Video->setPixmap(QPixmap::fromImage(image));
}

void Widget::getrect(Rect tmpselection)
{
    selectObject=true;
    selection=tmpselection;
}

void Widget::getcompletionsign(int selectiondone)
{
    trackObject=selectiondone;
    selectObject=false;
}

void Widget::on_camshift_clicked()
{
    if(selthm!='c')
    {
        selthm='c';
        connect(ui->Video,SIGNAL(sendrect(Rect)),this,SLOT(getrect(Rect)));
        connect(ui->Video,SIGNAL(selectiondone(int)),this,SLOT(getcompletionsign(int)));
    }
    else
    {
        selthm='o';
        selectObject=false;
        trackObject=0;
        trackWindow=Rect(0,0,0,0);
    }
}

void Widget::camshiftalgorithm(Mat &image)
{
    int hsize = 16;
    float hranges[] = {0,180};
    const float *phranges = hranges;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    if( trackObject )
    {
        int _vmin = 10, _vmax = 150 , smin=30;

        inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
                Scalar(180, 256, MAX(_vmin, _vmax)), mask);
        int ch[] = {0, 0};
        hue.create(hsv.size(), hsv.depth());
        mixChannels(&hsv, 1, &hue, 1, ch, 1);

        if( trackObject < 0 )
        {
            Mat roi(hue, selection), maskroi(mask, selection);
            calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
            normalize(hist, hist, 0, 255, NORM_MINMAX);
            trackWindow = selection;
            trackObject = 1;
        }

        calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        backproj &= mask;
        RotatedRect trackBox = CamShift(backproj, trackWindow,
                            TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
        if( trackWindow.area() <= 1 )
        {
            int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
            trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
                               trackWindow.x + r, trackWindow.y + r) &
                          Rect(0, 0, cols, rows);
        }

        ellipse( image, trackBox, Scalar(0,0,255), 3, LINE_AA );
    }

    if( selectObject && selection.width > 0 && selection.height > 0 )
    {
        Mat roi(image, selection);
        bitwise_not(roi, roi);
    }
}

QImage Widget::MatToQImage(const Mat &mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

