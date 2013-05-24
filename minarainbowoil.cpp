#include "minarainbowoil.h"

#include <QGraphicsProxyWidget>
#include <QDebug>
#include <cmath>

OilImageWidget::OilImageWidget(QWidget *parent) :
    QWidget(parent),
    _image(NULL)
{
    setAttribute(Qt::WA_StaticContents, true);
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);
}

void OilImageWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(_image)
        painter.drawImage(QPointF(), *_image);
}

void OilImageWidget::setImage(QImage *image)
{
    _image = image;
}

MinaRainbowOil::MinaRainbowOil(QObject *parent) :
    MinoAnimation(parent)
{
    _oilImageWidget = new OilImageWidget();
    _image = new QImage(_boundingRect.size(), QImage::Format_ARGB32);

    _itemGroup.addToGroup(_scene->addWidget(_oilImageWidget));

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");
    _generatorCurve->setEasingCurveType(QEasingCurve::InCirc);

    _style = new MinoItemizedProperty(this);
    _style->setObjectName("style");
    _style->setLabel("Style");
    _style->addItem("Vibration", 0);
    _style->addItem("Stop&Go", 1);
    _style->setCurrentItem("Vibration");

    _mprSpeed = new MinoPropertyReal(this);
    _mprSpeed->setLabel("Speed");
    _mprSpeed->setObjectName("speed");
    _mprSpeed->setValue(0.2);
    _mprBoost = new MinoPropertyReal(this);
    _mprBoost->setLabel("Boost");
    _mprBoost->setObjectName("boost");
    _mprBoost->setValue(5.0/15.0);
    _mprStep = new MinoPropertyReal(this);
    _mprStep->setLabel("Step");
    _mprStep->setObjectName("step");
    _mprStep->setValue(2.0/10.0);
    _pos = 0;
}

void MinaRainbowOil::renderImage(const qreal pos, const qreal hue, const qreal light, QImage *image)
{
    qreal sin_low = std::sin(pos * 0.0042);
    qreal sin_high = std::sin(pos * -0.042);

    const qreal stepX = _mprStep->value();
    const qreal stepY = _mprStep->value();

    qreal x_angle = 0.0;
    for (int x = 0; x < _boundingRect.width(); x++)
    {
        x_angle += stepX;

        qreal y_angle = 0.0;
        const qreal sin1 = std::sin(x_angle) * sin_low;
        for (int y = 0; y < _boundingRect.height(); y++)
        {
            y_angle += stepY;
            const qreal sin2 = std::sin(y_angle) * sin_high;
            const qreal sin3 = std::sin((x_angle + y_angle + (pos / 12.7)) / 2.0);
            qreal pixel_hue = hue + (sin1+sin2+sin3)/3.0;

            if(pixel_hue>1.0) {
                pixel_hue-=1.0;
            } else if (pixel_hue<0.0){
                pixel_hue+=1.0;
            }
            if((pixel_hue>1.0) || (pixel_hue<0.0)) {
                qDebug() << "pixel_hue" << pixel_hue
                         << "hue" << hue
                         << sin1 << sin2 << sin3;
            }
            pixel_hue = qBound(0.0, pixel_hue, 1.0);
            image->setPixel(x,y,QColor::fromHslF(pixel_hue, 1.0, light).rgb());
        }
    }
}

void MinaRainbowOil::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    QEasingCurve ec(_generatorCurve->easingCurveType());
    const qreal progress = ec.valueForProgress(_beatFactor->progressForGppqn(gppqn));

    qreal pos = _pos;
    pos += (_mprSpeed->value() * 5.0);
    _pos = pos;

    int style = _style->currentItem()->real();
    switch(style)
    {
    case 0:
        pos += (progress*(_mprBoost->value() * 15.0));
        break;
    case 1:
        pos += progress*(_mprBoost->value() * 15.0);
        _pos = pos;
        break;
    }

    renderImage(pos, _color->color().hueF(), _color->color().lightnessF(), _image);
    _oilImageWidget->setImage(_image);
}
