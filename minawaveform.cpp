#include "minawaveform.h"

#include <QDebug>

MinaWaveform::MinaWaveform(QObject *object) :
    MinoAnimation(object)
{
    _ecrHeight.setStartValue((qreal)_boundingRect.height()/2.0);
    _ecrHeight.setEndValue(1.0);
    _ecrHeight.setEasingCurve(QEasingCurve::OutBounce);

    _colorType = new MinoItemizedProperty(this);
    _colorType->setObjectName("color-type");
    _colorType->setLabel("Color type");
    _colorType->addItem("plain", 0);
    _colorType->addItem("grad.", 1);
    _colorType->setCurrentItem("plain");

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");
}

void MinaWaveform::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    // Colors
    QColor color = _color->color();
    QColor colorMin, colorMax;

    QLinearGradient grad(0.0, 0.0, 0.0, (qreal)_boundingRect.height()) ;
    _ecrHeight.setEasingCurve(_generatorCurve->easingCurveType());

    if (_colorType->currentItem()->real() == 0.0)
    {
        grad.setColorAt(0.0, Qt::transparent) ;
        grad.setColorAt(0.4, color) ;
        grad.setColorAt(0.6, color) ;
        grad.setColorAt(1.0, Qt::transparent) ;
    }
    else
    {
        qreal minValue = color.hueF()-0.15; if(minValue<0.0) minValue += 1.0;
        colorMin.setHsvF(minValue, 1.0, 1.0);

        qreal maxValue = color.hueF()+0.15; if(maxValue>1.0) maxValue -= 1.0;
        colorMax.setHsvF(maxValue, 1.0, 1.0);

        grad.setColorAt(0.0, colorMin) ;
        grad.setColorAt(0.3, color) ;
        grad.setColorAt(0.45, colorMax) ;
        grad.setColorAt(0.55, colorMax) ;
        grad.setColorAt(0.7, color) ;
        grad.setColorAt(1, colorMin) ;
    }

    const qreal middle = (qreal)_boundingRect.height()/2;
    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    for (int i=0; i<_boundingRect.width(); i++)
    {
        const qreal progress = _beatFactor->progressForGppqn(gppqn);
        qreal randHeight = qrandF() * _ecrHeight.valueForProgress(progress);
        _itemGroup.addToGroup(_scene->addLine(i, middle-randHeight, i, middle+randHeight, QPen(QBrush(grad),1)));
    }
}
