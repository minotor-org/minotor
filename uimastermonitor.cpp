#include "uimastermonitor.h"

#include <QBitmap>
#include <QPainter>
#include <QRect>
#include <QPaintEvent>
#include <QBrush>
#include <QDebug>
#include <QPen>

UiMasterMonitor::UiMasterMonitor(LedMatrix *ledMatrix, QWidget *parent) :
    QWidget(parent),
    _ledMatrix(ledMatrix)
{
    this->setAttribute(Qt::WA_OpaquePaintEvent);
    connect(ledMatrix, SIGNAL(updated()), this, SLOT(update()));
}

void UiMasterMonitor::paintEvent(QPaintEvent *event)
{

    (void)event;
    QSize sMatrix = _ledMatrix->size();
    // qDebug() << "sMatrix size" << sMatrix;
    QPainter painter(this);

    QRect rMatrix(0, 0, sMatrix.width(), sMatrix.height());
    QRect rWidget (0,0,width(),height());
    painter.drawImage(rWidget, *_ledMatrix->frame(), rMatrix);
    qreal stepX = (qreal)width() / sMatrix.width();
    qreal stepY = (qreal)height() / sMatrix.height();



    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    const int nbLines = (sMatrix.width()-1) + (sMatrix.height()-1);
    QLine lines[nbLines];
    int currentLine = 0;
    for (int x = 1; x < sMatrix.width(); x++)
    {
        int pos = x * stepX;
        lines[currentLine] = QLine(pos,0,pos,height());
        currentLine++;
    }
    for (int y = 1; y < sMatrix.height(); y++)
    {
        int pos = y * stepY;
        lines[currentLine] = QLine(0,pos,width(),pos);
        currentLine++;
    }
    painter.drawLines(lines,nbLines);

}

int UiMasterMonitor::heightForWidth( int width ) const
{
    return (qreal)(width * ((qreal)16/24));
}
