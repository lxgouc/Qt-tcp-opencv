#ifndef NEWQLABEL_H
#define NEWQLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <opencv2/opencv.hpp>

using namespace cv;

class NewQLabel : public QLabel
{
    Q_OBJECT

public:

    explicit NewQLabel(QWidget *parent=0);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

signals:
    void sendrect(Rect);

    void selectiondone(int);

private:
    QPoint startpoint;
    QPoint endpoint;
    Rect selection;

};

#endif // NEWQLABEL_H
