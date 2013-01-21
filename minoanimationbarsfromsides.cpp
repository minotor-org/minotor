#include "minoanimationbarsfromsides.h"
#include <QColor>
#include <QGraphicsScene>
#include <QDebug>

MinoAnimationBarsFromSides::MinoAnimationBarsFromSides(QGraphicsScene* scene, QObject *parent) :
    MinoAnimation(QString("Bars from sides"), scene, parent)
{
    _animatedProperty.setStartValue(QVariant(_scene->sceneRect().height()/2));
    _animatedProperty.setEndValue(QVariant(1.0));
    _animatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _beatFactor.setObjectName("Beat factor");
    _properties.append(&_beatFactor);
    _beatFactor.setValue(0.4); // BeatFactor 1

    _color.setObjectName("Color");
    _properties.append(&_color);

}
qreal MinoAnimationBarsFromSides::ratioToBeatFactor(qreal value)
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
    return tempoList.at(step);
}

void MinoAnimationBarsFromSides::animate(const unsigned int ppqn)
{
    const qreal beatFactor = ratioToBeatFactor(_beatFactor.value());;
    const unsigned int ppqnMax = (24*beatFactor);
    const unsigned int _ppqn = ppqn % ppqnMax;
    const qreal durationFactor = ((qreal)_ppqn / ppqnMax);
    const int currentTime = (qreal(_animatedProperty.duration())) * durationFactor;
    _animatedProperty.setCurrentTime(currentTime);

    QColor color(Qt::green);
    color.setRed(_color.value()*255);
    color.setBlue(_color.value()*255);

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    if (ppqn%48 < 24)
    {
        for (int i=0;i< _scene->sceneRect().height();i++)
        {
            int currentAnimatedPropertyValue = 1+(qrand()%_animatedProperty.currentValue().toInt());
            _itemGroup.addToGroup(_scene->addLine(0,i,(currentAnimatedPropertyValue),i,QPen(color)));
            _itemGroup.addToGroup(_scene->addLine((_scene->sceneRect().width()-currentAnimatedPropertyValue-1),i,(_scene->sceneRect().width()-1),i,QPen(color)));
        }
    }
    else
    {
        for (int i=0;i< _scene->sceneRect().width();i++)
        {
            int currentAnimatedPropertyValue = 1+(qrand()%_animatedProperty.currentValue().toInt());
            _itemGroup.addToGroup(_scene->addLine(i,0,i,(currentAnimatedPropertyValue),QPen(color)));
            _itemGroup.addToGroup(_scene->addLine(i,(_scene->sceneRect().height()-currentAnimatedPropertyValue-1),i,(_scene->sceneRect().height()-1),QPen(color)));
        }
    }

}
