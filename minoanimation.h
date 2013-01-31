#ifndef MINOANIMATION_H
#define MINOANIMATION_H

#include <QObject>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QGraphicsItemGroup>

#include "minoanimationproperty.h"

class Minotor;

class MinoAnimationDescription
{
public:
    explicit MinoAnimationDescription(const QString name, const QString tooltip, const QPixmap pixmap, const QString className) :
        _name(name),
        _tooltip(tooltip),
        _pixmap(pixmap),
        _className(className) { }

    const QString name() { return _name; }
    const QString tooltip() { return _tooltip; }
    const QPixmap pixmap() { return _pixmap; }
    const QString className() { return _className; }

private:
    QString _name;
    QString _tooltip;
    QPixmap _pixmap;
    QString _className;
};

class MinoAnimation : public QObject
{
    Q_OBJECT
public:
    explicit MinoAnimation(Minotor *minotor);

    virtual void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn) = 0;
    virtual const MinoAnimationDescription description() const = 0;

    const MinoAnimationPropertyList properties() { return _properties; }
    QGraphicsItemGroup* itemGroup() { return &_itemGroup; }

    static qreal qrandF() { return (qreal)qrand()/RAND_MAX; }
protected:
    QGraphicsScene *_scene;
    QRect _boundingRect;
    QGraphicsItemGroup _itemGroup;

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
