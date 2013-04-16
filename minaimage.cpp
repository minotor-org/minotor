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

MinaImage::MinaImage(QObject *parent) :
    MinoAnimation(parent),
    _imageIndex(0)
{
    // Color is not usable in this animation
    delete _color;
    _color = NULL;

    _imageWidget = new ImageWidget();
    _imageWidget->resize(_boundingRect.size());

    _itemGroup.addToGroup(_scene->addWidget(_imageWidget));

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _imageFilename = new MinoPropertyFilename(this);
    connect(_imageFilename, SIGNAL(filenameChanged(QString)), SLOT(loadFromFile(QString)));
    _imageFilename->setFilename("spaceinvader.gif");
}

void MinaImage::loadFromFile(const QString& filename)
{
    QImageReader ir;
    ir.setFileName(filename);

    if(ir.canRead())
    {
        foreach(QImage* i, _imageList)
        {
            delete i;
        }
        _imageList.clear();

        while(ir.canRead())
            _imageList.append(new QImage(ir.read()));

        if(_imageList.count())
            _imageWidget->setImage(_imageList.at(0));
    }
}

void MinaImage::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    if(_imageList.count())
    {
        if(_imageList.count() != 1)
        {
            QEasingCurve ecImageIndex(_generatorCurve->easingCurveType());
            const qreal pos = ecImageIndex.valueForProgress(_beatFactor->progressForGppqn(gppqn));
            int imageIndex = (pos*0.999999999*_imageList.count());
            if(imageIndex>=_imageList.count())
                imageIndex = _imageList.count()-1;
            if(_imageIndex != imageIndex)
            {
                _imageIndex = imageIndex;
                _imageWidget->setImage(_imageList.at(_imageIndex));
            }
        }
    }
}
