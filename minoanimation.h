#ifndef MINOANIMATION_H
#define MINOANIMATION_H

#include <QObject>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QGraphicsItemGroup>
#include <QPointF>

#include "minopropertycolor.h"
#include "minoitemizedproperty.h"
#include "minopropertybeat.h"
#include "minopersistentobject.h"

class MinoProgram;

class MinoAnimationDescription
{
public:
    explicit MinoAnimationDescription(const QString name, const QString tooltip, const QPixmap pixmap, const QString className) :
        _name(name),
        _tooltip(tooltip),
        _pixmap(pixmap),
        _className(className) { }

    const QString name() const { return _name; }
    const QString tooltip() const { return _tooltip; }
    const QPixmap pixmap() const { return _pixmap; }
    const QString className() const { return _className; }

private:
    QString _name;
    QString _tooltip;
    QPixmap _pixmap;
    QString _className;
};

class MinoAnimationGroup;

class MinoAnimation : public MinoPersistentObject
{
    Q_OBJECT
    friend class MinoAnimationGroup;

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled STORED true)
public:
    explicit MinoAnimation(QObject *parent);

    int id();

    virtual const MinoAnimationDescription description() const = 0;
    virtual QGraphicsItem* graphicItem() = 0;

    virtual void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn) = 0;

    static qreal qrandF() { return (qreal)qrand()/RAND_MAX; }
    QPointF qrandPointF();

    bool enabled() const { return _enabled; }
    MinoProgram* program() const { return _program; }
    MinoAnimationGroup* group() const { return _group; }
    void setGroup(MinoAnimationGroup *group);

    // MinoProperties
    // FIXME put this in MinoProperty and make MinoProperty inherited from MinoPersistentObject
    void setColor(const QColor& color) { if(_color) _color->setColor(color); }

    void setLoopSize(const QString& loopSize) { _beatFactor->setLoopSize(loopSize); }
    QString loopSize() const { return _beatFactor->loopSize(); }

public slots:
    void setEnabled(const bool enabled);

protected:
    // Parent
    MinoAnimationGroup *_group;
    MinoProgram *_program;

    // Graphics
    QGraphicsScene *_scene;
    QRect _boundingRect;

    // MinoAnimaBeat ?
    qreal ratioToBeatFactor(qreal value);
    MinoPropertyBeat *_beatFactor;
    MinoPropertyColor *_color;
    QPropertyAnimation _beatAnimatedProperty;
    void computeAnimaBeatProperty(const unsigned int gppqn);

    // Will be called by MinoAnimationGroup
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

    unsigned int startUppqn() const { return _startUppqn; }
    unsigned int duration() const { return _duration; }
    QGraphicsItem *graphicsItem() const { return _graphicsItem; }
    qreal progressForUppqn(const unsigned int uppqn) const { return (qreal)(uppqn - _startUppqn) / (qreal)_duration; }

    unsigned int _startUppqn;
    unsigned int _duration;
    QGraphicsItem *_graphicsItem;
} ;

typedef QList<MinoAnimatedItem> MinoAnimatedItems;
#endif // MINOANIMATION_H
