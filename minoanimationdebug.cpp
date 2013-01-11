#include "minoanimationdebug.h"
#include <QColor>
#include <QGraphicsScene>
#include <QDebug>

MinoAnimationDebug::MinoAnimationDebug(QGraphicsScene* scene, QObject *parent) :
    MinoAnimation(QString("Debug"), scene, parent)
{
    _animatedProperty.setStartValue(QVariant(1.0));
    _animatedProperty.setEndValue(QVariant(0.0));
    _animatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _beatFactor.setObjectName("Beat factor");
    _properties.append(&_beatFactor);

    _r.setObjectName("Red");
    _g.setObjectName("Green");
    _b.setObjectName("Blue");
    _properties.append(&_r);
    _properties.append(&_g);
    _properties.append(&_b);
}

qreal MinoAnimationDebug::ratioToBeatFactor(qreal value)
{
    /*
      1/4
      1/2
      1
      2
      4
      8
      16
      */
    QList<qreal> tempoList;
    tempoList.append(0.25);
    tempoList.append(0.5);
    tempoList.append(1);
    tempoList.append(2);
    tempoList.append(4);
    tempoList.append(8);
    tempoList.append(16);

    int step = (tempoList.count()-1) * value;
    //qDebug() << "value" << (qreal)value << "step" << step << "factor" << tempoList.at(step);
    return tempoList.at(step);
}

void MinoAnimationDebug::animate(const unsigned int ppqn)
{
    const qreal beatFactor = ratioToBeatFactor(_beatFactor.value());
    const unsigned int ppqnMax = ((qreal)24*beatFactor);
    const unsigned int _ppqn = ppqn % ppqnMax;
    const qreal durationFactor = ((qreal)_ppqn / ppqnMax);
    // qDebug() << "durationFactor" << durationFactor << "_ppqn" << _ppqn << "ppqnMax" << ppqnMax;
    const int currentTime = (qreal(_animatedProperty.duration())) * durationFactor;
    _animatedProperty.setCurrentTime(currentTime);
    QColor color(Qt::blue);
    color.setRed(_r.value()*255);
    color.setGreen(_g.value()*255);
    color.setBlue(_b.value()*255);

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
        delete item;
    }
    _itemGroup.addToGroup(_scene->addRect(QRectF(0, 0, 240, 160), QPen(color),QBrush(color) ));
    _itemGroup.setOpacity(_animatedProperty.currentValue().toFloat());

}
