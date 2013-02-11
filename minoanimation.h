#ifndef MINOANIMATION_H
#define MINOANIMATION_H

#include <QObject>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QGraphicsItemGroup>
#include <QPointF>

#include "minoproperty.h"
#include "minoitemizedproperty.h"
#include "minotextproperty.h"
#include "minopropertygroup.h"

class MinoProgram;

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
    friend class MinoProgram;

public:
    explicit MinoAnimation(MinoProgram *program);

    virtual const MinoAnimationDescription description() const = 0;
    virtual QGraphicsItem* graphicItem() = 0;
    const MinoPropertyGrouped propertyGrouped() { return _propertyGrouped; }

    virtual void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn) = 0;

    static qreal qrandF() { return (qreal)qrand()/RAND_MAX; }
    QPointF qrandPointF();

    bool enabled() const { return _enabled; }

public slots:
    void setEnabled(const bool enabled);
    void setDelayedEnabled(const bool enabled);

protected:
    // Parent
    MinoProgram *_program;

    // Graphics
    QGraphicsScene *_scene;
    QRect _boundingRect;

    MinoPropertyGrouped _propertyGrouped;
    MinoPropertyList _mplMain;
    // MinoAnimaBeat ?
    qreal ratioToBeatFactor(qreal value);
    MinoItemizedProperty _beatFactor;
    MinoProperty _color;
    QPropertyAnimation _beatAnimatedProperty;
    void computeAnimaBeatProperty(const unsigned int gppqn);

    // Will be called by channel
    void _setEnabled(const bool on);
    bool _enabled;
    bool _pending;
signals:
    void enabledChanged(bool on);
    void pendingChanged(bool on);
};

typedef QList<MinoAnimation*> MinoAnimationList;

class MinoAnimatedItem
{
public:
    explicit MinoAnimatedItem(const unsigned int startUppqn,const unsigned int duration,QGraphicsItem *graphicsItem):
        _startUppqn(startUppqn),
        _duration(duration),
        _graphicsItem(graphicsItem)
    {
    }
    unsigned int _startUppqn;
    unsigned int _duration;
    QGraphicsItem *_graphicsItem;

} ;

typedef QList<MinoAnimatedItem> MinoAnimatedItems;
#endif // MINOANIMATION_H
