#include "uipixeledview.h"

#include <QRect>
#include <QLine>
#include <QDebug>

UiPixeledView::UiPixeledView(QGraphicsView *view, QWidget *parent) :
    QWidget(parent),
    _view(view)
{
    this->setAttribute(Qt::WA_OpaquePaintEvent);
    _image = new QImage(24, 16, QImage::Format_RGB32);
}

UiPixeledView::~UiPixeledView()
{
    delete _image;
}

void UiPixeledView::refresh()
{
    _image->fill(Qt::black);
    QPainter painter(_image);
    _view->render(&painter, QRectF(0,0,_image->width(),_image->height()), _view->viewport()->rect(), Qt::IgnoreAspectRatio);
    //_view->render(&painter);
    /*
    qDebug() << "UiPixeledView"
             << "painter" << painter.viewport()
             << "viewport rect:" << _view->viewport()->rect()
             << "scene rect:" << _view->sceneRect();
             */
    update();
}

void UiPixeledView::paintEvent(QPaintEvent *event)
{
    (void)event;
    QSize sMatrix(24, 16);

    QPainter painter(this);

    QRect rMatrix(0, 0, sMatrix.width(), sMatrix.height());
    QRect rWidget (0, 0, width(), height());
    painter.drawImage(rWidget, *_image, rMatrix);
    qreal stepX = (qreal)width() / sMatrix.width();
    qreal stepY = (qreal)height() / sMatrix.height();

    //painter.setBrush(QBrush(Qt::black));

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

int UiPixeledView::heightForWidth( int width ) const
{
    return (qreal)(width * ((qreal)16/24));
}
