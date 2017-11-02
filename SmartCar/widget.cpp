#include "widget.h"
#include "ui_widget.h"
#include "worker.h"
#include <QDebug>
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
    autotrack=false;
    selthm='o';
    trackObject=0;
    //receivedbytes=0;
    //framesize=0;
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(acceptconnection()));
    connect(ui->SwitchButton,SIGNAL(clicked()),this,SLOT(opencamara()));
    threadinit();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::threadinit()
{
    Worker *mythread=new Worker;
    mythread->moveToThread(&workthread);
    connect(&workthread,SIGNAL(started()),mythread,SLOT(deleteLater()));
    connect(&workthread,SIGNAL(finished()),mythread,SLOT(netinit()));
    connect(this,SIGNAL(dirsignal(QString,int)),mythread,SLOT(dirdata(QString,int)));
    //connect(this,SIGNAL(drisignal()),mythread,SLOT(autodridata()));
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
       workthread.start();
       ui->SwitchButton->setText("OFF");
       selthm='o';
    }
    else
    {
        if(tcpserver->isListening())
            tcpserver->close();
        else
            tcpsocket->disconnectFromHost();
        workthread.quit();
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
    case 'f':
        detectAndDraw( frame, cascade, nestedCascade, 2, false );
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
        disconnect(ui->Video,SIGNAL(sendrect(Rect)),this,SLOT(getrect(Rect)));
        disconnect(ui->Video,SIGNAL(selectiondone(int)),this,SLOT(getcompletionsign(int)));
    }
}

void Widget::on_facedetect_clicked()
{
    if(selthm!='f')
    {
        selthm='f';
        if ( !nestedCascade.load( "/home/lxg/ouc/Qt-tcp-opencv/SmartCar/haarcascade_eye_tree_eyeglasses.xml" ) )
            cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
        if( !cascade.load( "/home/lxg/ouc/Qt-tcp-opencv/SmartCar/haarcascade_frontalface_alt.xml" ) )
        {
            cerr << "ERROR: Could not load classifier cascade" << endl;
            exit(1);
        }
    }
    else
        selthm='o';
}

void Widget::on_UpButton_clicked()
{
    emit dirsignal("up",000);
}

void Widget::on_LeftButton_clicked()
{
    emit dirsignal("up",000);
}

void Widget::on_RightButton_clicked()
{
    emit dirsignal("up",000);
}

void Widget::on_DownButton_clicked()
{
    emit dirsignal("up",000);
}

void Widget::on_AutoDrive_clicked()
{
    autotrack=!autotrack;
}

void Widget::poscal()
{
    /*
     * */
    //emit drisignal();
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

void Widget::detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, bool tryflip )
{
    double t = 0;
    vector<Rect> faces, faces2;
    const static Scalar colors[] =
    {
        Scalar(255,0,0),
        Scalar(255,128,0),
        Scalar(255,255,0),
        Scalar(0,255,0),
        Scalar(0,128,255),
        Scalar(0,255,255),
        Scalar(0,0,255),
        Scalar(255,0,255)
    };
    Mat gray, smallImg;

    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;
    cv::resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)getTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(30, 30) );
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CASCADE_FIND_BIGGEST_OBJECT
                                 //|CASCADE_DO_ROUGH_SEARCH
                                 |CASCADE_SCALE_IMAGE,
                                 Size(30, 30) );
        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r )
        {
            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }
    t = (double)getTickCount() - t;
    printf( "detection time = %g ms\n", t*1000/getTickFrequency());
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;

        double aspect_ratio = (double)r.width/r.height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r.x + r.width*0.5)*scale);
            center.y = cvRound((r.y + r.height*0.5)*scale);
            radius = cvRound((r.width + r.height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        else
            rectangle( img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
                       cvPoint(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
                       color, 3, 8, 0);
        if( nestedCascade.empty() )
            continue;
        smallImgROI = smallImg( r );
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CASCADE_FIND_BIGGEST_OBJECT
            //|CASCADE_DO_ROUGH_SEARCH
            //|CASCADE_DO_CANNY_PRUNING
            |CASCADE_SCALE_IMAGE,
            Size(30, 30) );
        for ( size_t j = 0; j < nestedObjects.size(); j++ )
        {
            Rect nr = nestedObjects[j];
            center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
            center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
            radius = cvRound((nr.width + nr.height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
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






