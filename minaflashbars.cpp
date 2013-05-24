#include "minaflashbars.h"

#include <QDebug>

MinaFlashBars::MinaFlashBars(QObject *object) :
    MinoAnimation(object)
{
    _ecrAlpha.setStartValue(1.0);
    _ecrAlpha.setEndValue(0.0);
    _ecrAlpha.setEasingCurve(QEasingCurve::OutBounce);

    _width = new MinoPropertyReal(this);
    _width->setObjectName("width");
    _width->setLabel("Width");
    _width->setValue(0.2);

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    int posY = qMin(qrandF(),(1.0-(_width->value())))*_boundingRect.height();
    _oldPosY = posY;
    _itemGroup.addToGroup(_scene->addRect(0, posY, _boundingRect.width(), _width->value()*_boundingRect.height(), QPen(Qt::NoPen),QBrush(_color->color())));
}

void MinaFlashBars::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    const unsigned int b = _beatFactor->loopSizeInPpqn();

    QColor color = _color->color();

    _ecrAlpha.setEasingCurve(_generatorCurve->easingCurveType());

    color.setAlphaF(_ecrAlpha.valueForProgress(_beatFactor->progressForGppqn(gppqn)));

    QColor transparency;
    transparency.setAlpha(0);

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
        static_cast<QGraphicsRectItem*>(item)->setBrush(color);
    }

    if ((gppqn%b)==0)
    {
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
           delete item;
        }
        int posY = qMin(qrandF(),(1.0-(_width->value())))*_boundingRect.height();

        //TODO : find a better way to avoid identical consecutive position
        if (_width->value()*_boundingRect.height()<_boundingRect.height()-1)
        {
            while (posY == _oldPosY)
            {
                posY = qMin(qrandF(),(1.0-(_width->value())))*_boundingRect.height();
            }
        }
        _oldPosY = posY;
        _itemGroup.addToGroup(_scene->addRect(0, posY, _boundingRect.width(), _width->value()*_boundingRect.height(), QPen(Qt::NoPen),QBrush(color)));
    }
}
