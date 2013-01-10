#ifndef MINOANIMATION_H
#define MINOANIMATION_H

#include <QObject>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QGraphicsItemGroup>

#include "minoanimationproperty.h"

class MinoAnimation : public QObject
{
    Q_OBJECT
public:
    explicit MinoAnimation(QString name, QGraphicsScene *scene, QObject *parent = 0);
    virtual void animate(const unsigned int ppqn) = 0;
    QString name() { return _name; }
    QGraphicsItemGroup* itemGroup() { return &_itemGroup; }
    MinoAnimationPropertyList properties() { return _properties; }
protected:
    QGraphicsScene *_scene;
    QPropertyAnimation _animatedProperty;
    QGraphicsItemGroup _itemGroup;
    QString _name;
    MinoAnimationPropertyList _properties;
signals:
    
public slots:
    
};

typedef QList<MinoAnimation*> MinoAnimationList;

#endif // MINOANIMATION_H
