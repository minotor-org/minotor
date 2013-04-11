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
    // Dirty rescale of previously saved image
    QImage image = _image.scaled(event->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    _image = image;
}

void ImageWidget::setImage(QImage *image)
{
    _image = image->scaled(size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    qDebug() << Q_FUNC_INFO
                << image->size()
                << _image.size();
}

MinaImage::MinaImage(MinoAnimationGroup *parent) :
    MinoAnimation(parent)
{
    _imageWidget = new ImageWidget();
    _imageWidget->resize(_boundingRect.size());

    QImageReader ir;
    ir.setFileName("spaceinvader.gif");

    while(ir.canRead())
        _imageList.append(new QImage(ir.read()));

    if(_imageList.count())
        _imageWidget->setImage(_imageList.at(0));

    _itemGroup.addToGroup(_scene->addWidget(_imageWidget));

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
}

void MinaImage::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    computeAnimaBeatProperty(gppqn);
    if(_imageList.count())
    {
        _beatAnimatedProperty.setEasingCurve(_generatorCurve->easingCurveType());
        const qreal pos = _beatAnimatedProperty.currentValue().toReal();
        int imageIndex = (pos*0.999999999*_imageList.count());
        if(imageIndex>=_imageList.count())
            imageIndex = _imageList.count()-1;
        if(_imageIndex != imageIndex)
        {
            _imageIndex = imageIndex;
            _imageWidget->setImage(_imageList.at(_imageIndex));
        }
        /*
        qDebug() << Q_FUNC_INFO
                 << "gppqn" << gppqn
                 << "pos factor:" << (qreal)pos
                 << "image index:" << imageIndex;
                 */
    }
}
