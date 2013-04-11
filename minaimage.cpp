#include "minaimage.h"

#include <QGraphicsProxyWidget>
#include <QDebug>

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
}

void ImageWidget::paintEvent(QPaintEvent *)
{
    qDebug() << Q_FUNC_INFO;

    QPainter painter(this);
    painter.drawImage(QPointF(), _image);
}
void ImageWidget::resizeEvent(QResizeEvent *event)
{
    QImage picture(QString("spaceinvader.gif"));
    _image = picture.scaled(event->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    qDebug() << Q_FUNC_INFO
                << picture.size()
                << _image.size();
}

MinaImage::MinaImage(MinoAnimationGroup *parent) :
    MinoAnimation(parent)
{
    QWidget *_imageWidget = new ImageWidget();
    _imageWidget->resize(_boundingRect.size());
    _itemGroup.addToGroup(_scene->addWidget(_imageWidget));
}

void MinaImage::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
}
