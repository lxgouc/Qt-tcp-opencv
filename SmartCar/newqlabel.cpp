#include "newqlabel.h"
#include <QDebug>

NewQLabel::NewQLabel(QWidget *parent):
    QLabel(parent)
{

}


void NewQLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        startpoint=event->pos();
        selection=Rect(startpoint.x(),startpoint.y(),0,0);
    }
}

void NewQLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        endpoint=event->pos();
        update();
        if(qAbs(selection.width)>0 && qAbs(selection.height)>0)
            emit sendrect(selection);
    }
}

void NewQLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        //selection=Rect(0,0,0,0);
        startpoint=QPoint(0,0);
        endpoint=QPoint(0,0);
        update();
    }
    emit selectiondone(-1);
}

void NewQLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    selection.x = startpoint.x();
    selection.y = startpoint.y();
    selection.width = endpoint.x() - startpoint.x();
    selection.height = endpoint.y() - startpoint.y();
    //selection &= Rect(0, 0, this->width(), this->height());
    QPainter painter;
    painter.begin(this);
    painter.drawRect(startpoint.x(),startpoint.y(),selection.width,selection.height);
    painter.end();
    qDebug()<<"lxg";
}
