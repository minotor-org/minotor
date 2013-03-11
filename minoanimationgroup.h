#ifndef MINOANIMATIONGROUP_H
#define MINOANIMATIONGROUP_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QList>

#include "minoanimation.h"

class MinoProgram;

class MinoAnimationGroup : public QObject
{
    Q_OBJECT
    friend class MinoProgram;

public:
    explicit MinoAnimationGroup(MinoProgram *parent);
/*
    ~MinoAnimationGroup() {
        foreach (MinoAnimation *animation, _minoAnimations)
        {
            delete(animation);
        }
    }
*/
    // Accessors
    QPixmap screenshot() const { return _screenshot; }
    MinoAnimationList animations() const { return _animations; }
    MinoProgram *program() const { return _program; }
    QGraphicsItemGroup *itemGroup() { return &_itemGroup; }
    bool enabled() const { return _enabled; }

    // Animation manipulation
    void addAnimation(MinoAnimation *animation);
    MinoAnimation* addAnimation(const QString animationClassName);
    void moveAnimation(int oldIndex, int newIndex);

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
    // Signal emitted when animations have been changed (ie. create, delete, moved)
    void updated();

    // Signal emitted when group is enabled
    void enabledChanged(bool on);
public slots:
    void setDelayedEnabled(const bool enabled);
    void setEnabled(bool on);
};

typedef QList<MinoAnimationGroup*> MinoAnimationGroupList;

#endif // MINOANIMATIONGROUP_H
