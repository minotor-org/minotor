#include "minoanimationplasma.h"
#include <QColor>
#include <QGraphicsScene>
#include <QDebug>
#include <math.h>


MinoAnimationPlasma::MinoAnimationPlasma(QGraphicsScene* scene, QObject *parent) :
    MinoAnimation(QString("Bars from sides"), scene, parent)
{
    _animatedProperty.setStartValue(QVariant(100));
    _animatedProperty.setEndValue(QVariant(10));
    _animatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _beatFactor.setObjectName("Beat factor");
    _properties.append(&_beatFactor);
    _beatFactor.setValue(0.4); // BeatFactor 1

    _color.setObjectName("Color");
    _properties.append(&_color);

    for (int i=0;i<160;i+=10)
    {
        for (int j=0; j<240;j+=10)
        {
            _itemGroup.addToGroup(_scene->addLine ( j, i, j+1, i+1, QPen(Qt::red) ));
        }
    }

}
qreal MinoAnimationPlasma::ratioToBeatFactor(qreal value)
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

void MinoAnimationPlasma::animate(const unsigned int ppqn)
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
    color.setAlpha(_animatedProperty.currentValue().toInt());
    // qDebug() << color.alpha();

    QColor transparency;
    transparency.setAlpha(0);

    foreach (QGraphicsItem *item , _itemGroup.childItems())
    {
        static_cast<QGraphicsLineItem*>(item)->setPen(QPen(transparency));
        qDebug() << "set transparency";
    }



    const qreal pixelCount = 24*16;
    for(int i=0; i<pixelCount; i++)
    {
            qDebug() << "set new color";
            int sinColor = int(128.0 + (128.0 * sin(i / 8.0)));
            color.setBlue(sinColor);
            // qDebug() << "x" << x << "y" << y;
            static_cast<QGraphicsLineItem*>(_itemGroup.childItems().at(i))->setPen(QPen(color));
            //_itemGroup.addToGroup(_scene->addLine( x, y, x+1, y+1, QPen(Qt::green)));

    }
}
