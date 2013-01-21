#include "minoanimationwaveform.h"
#include <QDebug>

MinoAnimationWaveform::MinoAnimationWaveform(QGraphicsScene *scene, QObject *parent) :
    MinoAnimation(QString("Waveform"), scene, parent)
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
qreal MinoAnimationWaveform::ratioToBeatFactor(qreal value)
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

void MinoAnimationWaveform::animate(const unsigned int ppqn)
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

    qDebug() << _scene->sceneRect().height();
    qDebug() << _scene->sceneRect().width();

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    for (int i=0;i<_scene->sceneRect().width();i++)
    {
        int currentAnimatedPropertyValue = 1+(qrand()%_animatedProperty.currentValue().toInt());

        qDebug() << currentAnimatedPropertyValue;
        //_itemGroup.addToGroup(_scene->addRect(i*10,80-currentAnimatedPropertyValue,10,currentAnimatedPropertyValue*2,QPen(color),QBrush(Qt::NoBrush)));
        _itemGroup.addToGroup(_scene->addLine(i,((_scene->sceneRect().height()/2)-currentAnimatedPropertyValue),i,((_scene->sceneRect().height()/2)+currentAnimatedPropertyValue),QPen(color)));
    }

}
