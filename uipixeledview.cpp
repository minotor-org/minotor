#include "uipixeledview.h"

#include <QPainter>
#include <QLine>
#include <QDebug>

UiPixeledView::UiPixeledView(QGraphicsScene *scene, QWidget *parent) :
    QWidget(parent),
    _scene(scene)
{
    // Optimize widget's repaint
    setAttribute(Qt::WA_OpaquePaintEvent);

    // Set default values
    _image = NULL;
    setMatrixSize(QSize(32, 24));
    setViewRect(QRect(0, 0, 32, 24));
}

UiPixeledView::~UiPixeledView()
{
    delete _image;
}

void UiPixeledView::refresh()
{
    // Set background
    _image->fill(Qt::black);

    // Render the scene
    QPainter painter(_image);
    _scene->render(&painter, QRectF(_image->rect()), _viewRect, Qt::IgnoreAspectRatio);

    qDebug() << "UiPixeledView"
             << "painter" << painter.viewport()
             << "view rect:" << _viewRect
             << "scene rect:" << _scene->sceneRect();

    // Request a QWidget update
    update();
}

void UiPixeledView::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter painter(this);

    QRect rMatrix(0, 0, _matrixSize.width(), _matrixSize.height());
    painter.drawImage(rect(), *_image, rMatrix);

    qreal stepX = (qreal)width() / _matrixSize.width();
    qreal stepY = (qreal)height() / _matrixSize.height();

    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    const int nbLines = (_matrixSize.width()-1) + (_matrixSize.height()-1);
    QLine lines[nbLines];
    int currentLine = 0;
    for (int x = 1; x < _matrixSize.width(); x++)
    {
        int pos = x * stepX;
        lines[currentLine] = QLine(pos,0,pos,height());
        currentLine++;
    }
    for (int y = 1; y < _matrixSize.height(); y++)
    {
        int pos = y * stepY;
        lines[currentLine] = QLine(0,pos,width(),pos);
        currentLine++;
    }
    painter.drawLines(lines,nbLines);
}

int UiPixeledView::heightForWidth( int width ) const
{
    return ((qreal)width * _matrixRatio);
}

void UiPixeledView::setMatrixSize(const QSize matrixSize)
{
    _matrixSize = matrixSize;
    _matrixRatio = (qreal)_matrixSize.height() / (qreal)_matrixSize.width();
    if (_image) delete _image;
    _image = new QImage(_matrixSize, QImage::Format_RGB32);
}
