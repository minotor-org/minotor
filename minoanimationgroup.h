#ifndef MINOANIMATIONGROUP_H
#define MINOANIMATIONGROUP_H

#include "minopersistentobject.h"

#include <QGraphicsItemGroup>
#include <QList>

#include "minoanimation.h"

class MinoProgram;

class MinoAnimationGroup : public MinoPersistentObject
{
    Q_OBJECT
    friend class MinoProgram;

public:
    explicit MinoAnimationGroup(MinoProgram *parent);

    ~MinoAnimationGroup();

    // Accessors
    int id();
    QPixmap screenshot() const { return _screenshot; }
    MinoAnimationList animations() const { return _animations; }
    MinoProgram *program() const { return _program; }
    QGraphicsItemGroup *itemGroup() { return &_itemGroup; }
    bool enabled() const { return _enabled; }

    // Animation manipulation
    void addAnimation(MinoAnimation *animation);
    MinoAnimation* addAnimation(const QString animationClassName, int index = -1);
    void moveAnimation(int oldIndex, int newIndex);
    MinoAnimation* takeAnimationAt(int index);
    void insertAnimation(MinoAnimation *animation, int index = -1);

private:
    QPixmap _screenshot;
    MinoAnimationList _animations;
    bool _enabled;
    MinoProgram *_program;
    QGraphicsItemGroup _itemGroup;

    // Will be called by MinoProgram
    void _setEnabled(const bool on);

private slots:
    void destroyAnimation(QObject *animation);

signals:
    // Signal emitted when group is enabled
    void enabledChanged(bool on);
    void animationAdded();
public slots:
    void setDelayedEnabled(const bool enabled);
    void setEnabled(bool on);
    void toogle() { _setEnabled(!_enabled); }
};

typedef QList<MinoAnimationGroup*> MinoAnimationGroupList;

#endif // MINOANIMATIONGROUP_H
