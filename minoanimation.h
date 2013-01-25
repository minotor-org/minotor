#ifndef MINOANIMATION_H
#define MINOANIMATION_H

#include <QObject>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QGraphicsItemGroup>

#include "minoanimationproperty.h"

class MinoChannel;

class MinoAnimation : public QObject
{
    Q_OBJECT
public:
    explicit MinoAnimation(QString name, MinoChannel *parent);

    void setChannel(MinoChannel *channel);

    virtual void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn) = 0;

    QString name() { return _name; }
    QGraphicsItemGroup* itemGroup() { return &_itemGroup; }
    MinoAnimationPropertyList properties() { return _properties; }

protected:
    QGraphicsScene *_scene;
    QRect _boundingRect;

    QGraphicsItemGroup _itemGroup;
    QString _name;
    MinoAnimationPropertyList _properties;

    // MinoAnimaBeat ?
    qreal ratioToBeatFactor(qreal value);
    MinoAnimationProperty _beatFactor;
    QPropertyAnimation _beatAnimatedProperty;
    void computeAnimaBeatProperty(const unsigned int gppqn);
signals:
    
public slots:
    
};

typedef QList<MinoAnimation*> MinoAnimationList;

#endif // MINOANIMATION_H
