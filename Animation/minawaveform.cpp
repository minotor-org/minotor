#include "minawaveform.h"

#include <QDebug>

MinaWaveform::MinaWaveform(QObject *object) :
    MinoAnimation(object)
{
    _linesHeight = new qreal[_boundingRect.width()];

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

    _animationType = new MinoItemizedProperty(this);
    _animationType->setObjectName("anim-type");
    _animationType->setLabel("Type");
    _animationType->addItem("rand.", 0);
    _animationType->addItem("smooth", 1);
    _animationType->setCurrentItem("rand.");

    for (int i=0; i<_boundingRect.width(); i++)
    {
        _itemGroup.addToGroup(_scene->addLine(QLineF()));
    }
}

MinaWaveform::~MinaWaveform()
{
    delete [] _linesHeight;
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

    for (int i=0;i<_itemGroup.childItems().count();i++)
    {
        QGraphicsItem* item = _itemGroup.childItems().at(i);

        QGraphicsLineItem *line = dynamic_cast<QGraphicsLineItem*>(item);

        const qreal progress = _beatFactor->progressForGppqn(gppqn);
        if ((_animationType->currentItem()->real() == 0.0) || (_beatFactor->isBeat(gppqn)))
        {
            qreal randHeight = qrandF() * _ecrHeight.valueForProgress(progress);
            line->setLine(i,middle-randHeight,i,middle+randHeight);
            _linesHeight[i] = randHeight;
        }

        qreal height = _linesHeight[i]*(1.0-_generatorCurve->easingCurve().valueForProgress(progress));

        line->setLine(i,middle+0.1-height,i,middle+height);
        line->setPen(QPen(QBrush(grad),1));
    }

}
