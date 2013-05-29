#include "minarandompixels.h"

#include <QDebug>

MinaRandomPixels::MinaRandomPixels(QObject *object) :
    MinoAnimation(object)
{
    _ecrAlpha.setStartValue(1.0);
    _ecrAlpha.setEndValue(0.0);
    _ecrAlpha.setEasingCurve(QEasingCurve::OutBounce);

    _density = new MinoPropertyReal(this);
    _density->setObjectName("density");
    _density->setLabel("Density");
    _density->setValue(0.1);

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    for (qreal i=0.0;i<_boundingRect.height();i+=1.0)
    {
        for (qreal j=0.0; j<_boundingRect.width();j+=1.0)
        {
            const qreal h = 0.1;
            _itemGroup.addToGroup(_scene->addLine ( j, i, j+h, i+h, QPen(Qt::NoPen) ));
        }
    }
}

void MinaRandomPixels::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)qn;

    QColor color = _color->color();

    _ecrAlpha.setEasingCurve(_generatorCurve->easingCurveType());

    color.setAlphaF(_ecrAlpha.valueForProgress(_beatFactor->progressForGppqn(gppqn)));

    QColor transparency;
    transparency.setAlpha(0);

    if ((ppqn%6) == 0)
    {
        foreach (QGraphicsItem *item , _itemGroup.childItems())
        {
            static_cast<QGraphicsLineItem*>(item)->setPen(QPen(transparency));
        }

        const qreal pixelCount = _density->value()*(_boundingRect.width()*_boundingRect.height());
        for(int i=0; i<pixelCount; i++)
        {
            int x = (qrandF()*_boundingRect.width());
            int y = (qrandF()*_boundingRect.height());

            int pixelIndex = (y*_boundingRect.width())+x;

            static_cast<QGraphicsLineItem*>(_itemGroup.childItems().at(pixelIndex))->setPen(QPen(color));
        }
    }
}
