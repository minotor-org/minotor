#ifndef MINOANIMATION_H
#define MINOANIMATION_H

#include <QObject>

#include "minotor.h"
#include "minoanimationproperty.h"

class MinoAnimation : public QObject
{
    Q_OBJECT
public:
    explicit MinoAnimation(Minotor *parent);
    virtual void animate(const unsigned int ppqn) = 0;
    QGraphicsItemGroup* itemGroup();

    MinoAnimationPropertyList properties() { return _properties; };

protected:
    QPropertyAnimation _animatedProperty;
    QGraphicsItemGroup _itemGroup;

    MinoAnimationPropertyList _properties;
signals:
    
public slots:
    
};

typedef QList<MinoAnimation*> MinoAnimationList;

#endif // MINOANIMATION_H
