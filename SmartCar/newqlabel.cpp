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
        selection &= Rect(0, 0, this->width(), this->height());
        //selection.x+=1;
        //selection.y+=11;
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
    if(selection.width>0 && selection.height>0)
    {
        emit selectiondone(-1);
    }
}

void NewQLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    selection.x = qMin(endpoint.x(),startpoint.x());
    selection.y = qMin(endpoint.y(),startpoint.y());
    selection.width = qAbs(endpoint.x() - startpoint.x());
    selection.height = qAbs(endpoint.y() - startpoint.y());
    QPainter painter;
    painter.begin(this);
    painter.drawRect(selection.x,selection.y,selection.width,selection.height);
    painter.end();
}
