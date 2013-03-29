#include "minabarsfromsides.h"
#include <QColor>
#include <QDebug>

MinaBarsFromSides::MinaBarsFromSides(MinoAnimationGroup *group) :
    MinoAnimation(group)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _colorType = new MinoItemizedProperty(this);
    _colorType->setObjectName("color type");
    _colorType->addItem("plain", 0);
    _colorType->addItem("grad.", 1);
    _colorType->setCurrentItem("plain");
    _mplLine2.append(_colorType);

    _propertyGrouped.append(&_mplLine2);

}

void MinaBarsFromSides::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    computeAnimaBeatProperty(gppqn);

    // Colors
    QColor color, colorMin, colorMax;
    color.setHsvF(_color->value(), 1.0, 1.0);

    QLinearGradient gradH(0.0, 0.0, 0.0, (qreal)_boundingRect.height()) ;
    QLinearGradient gradV(0.0, 0.0, (qreal)_boundingRect.width(), 0.0) ;

    if (_colorType->currentItem()->real() == 0.0)
    {
        gradH.setColorAt(0.0, color) ;
        gradH.setColorAt(0.5, Qt::transparent) ;
        gradH.setColorAt(1.0, color) ;

        gradV.setColorAt(0.0, color) ;
        gradV.setColorAt(0.5, Qt::transparent) ;
        gradV.setColorAt(1.0, color) ;
    }
    else
    {
        qreal minValue = _color->value()-0.15; if(minValue<0.0) minValue += 1.0;
        colorMin.setHsvF(minValue, 1.0, 1.0);

        qreal maxValue = _color->value()+0.15; if(maxValue>1.0) maxValue -= 1.0;
        colorMax.setHsvF(maxValue, 1.0, 1.0);

        gradH.setColorAt(0.0, colorMax) ;
        gradH.setColorAt(0.3, color) ;
        gradH.setColorAt(0.45, colorMin) ;
        gradH.setColorAt(0.55, colorMin) ;
        gradH.setColorAt(0.7, color) ;
        gradH.setColorAt(1, colorMax) ;

        gradV.setColorAt(0.0, colorMax) ;
        gradV.setColorAt(0.3, color) ;
        gradV.setColorAt(0.45, colorMin) ;
        gradV.setColorAt(0.55, colorMin) ;
        gradV.setColorAt(0.7, color) ;
        gradV.setColorAt(1, colorMax) ;
    }

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    if ((int)(gppqn/_beatFactor->currentItem()->real())%2)
    {
        for (int i=0;i< _boundingRect.height();i++)
        {
            qreal lineLength = 0.5 + (qrandF() * _beatAnimatedProperty.currentValue().toReal() * _boundingRect.width() / 2.0);
            _itemGroup.addToGroup(_scene->addLine(_boundingRect.left(), i, _boundingRect.left()+lineLength, i, QPen(QBrush(gradV),1)));
            _itemGroup.addToGroup(_scene->addLine(_boundingRect.width()-lineLength, i, _boundingRect.width(), i, QPen(QBrush(gradV),1)));
        }
    }
    else
    {
        for (int i=0;i< _boundingRect.width();i++)
        {
            qreal lineLength = 0.5 + (qrandF() * _beatAnimatedProperty.currentValue().toReal() * _boundingRect.height() / 2.0);
            _itemGroup.addToGroup(_scene->addLine(i, _boundingRect.top(), i, _boundingRect.top()+lineLength, QPen(QBrush(gradH),1)));
            _itemGroup.addToGroup(_scene->addLine(i, (qreal)_boundingRect.height()-lineLength, i, _boundingRect.height(), QPen(QBrush(gradH),1)));
        }
    }
}
