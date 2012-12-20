#include "monitor.h"
#include <QBitmap>
#include <QPainter>
#include <QRect>
#include <QPaintEvent>
#include <QBrush>


Monitor::Monitor(QWidget *parent) :
    QWidget(parent),
    _ledMatrix(NULL)

{

}

void Monitor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if(_ledMatrix)
    {

        QRect fullRect (0,0,this->width(),this->height());
        painter.drawImage(fullRect, *_ledMatrix->frame(), QRect(0,0,_ledMatrix->frame()->width(), _ledMatrix->frame()->height()));
        //painter.drawRect(10,10,10,10);
        qreal pasX = (this->width() / _ledMatrix->frame()->width());
        qreal pasY = (this->height() / _ledMatrix->frame()->height());
        painter.setBrush(QBrush(Qt::black));
        painter.setPen(QPen(Qt::NoPen));
        for (int i = 1;i<_ledMatrix->frame()->width();i++)
        {
            int pos = (i * pasX) - (1);
            painter.drawRect(QRect(pos,0,2,this->height()));
        }
        for (int i = 1;i<_ledMatrix->frame()->height();i++)
        {
            int pos = (i * pasY) - (1);
            painter.drawRect(QRect(0,pos,this->width(),2));
        }
        //painter.drawRect(QRect(10,0,2,this->height()));
    }
}

void Monitor::setLedMatrix(LedMatrix *matrix)
{
    _ledMatrix = matrix;

}
