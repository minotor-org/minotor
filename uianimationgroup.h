#ifndef UIANIMATIONGROUP_H
#define UIANIMATIONGROUP_H

#include <QGroupBox>
#include <QPushButton>
#include <QLayout>

#include "minoanimationgroup.h"
#include "minoanimation.h"
#include "uianimation.h"

class UiAnimationGroup : public QGroupBox
{
    Q_OBJECT
public:
    explicit UiAnimationGroup(MinoAnimationGroup *group, QWidget *parent);
    ~UiAnimationGroup();

    void setExpanded(bool on);
    void addAnimation(MinoAnimation *animation, int index = -1);
    UiAnimation *takeAt(int index);
    void moveAnimation(int srcId, int destId);
    void insertAnimation(UiAnimation *animation, int destId);
    // Accessors
    MinoAnimationGroup* group() const { return _group; }

private:
    MinoAnimationGroup *_group;
    QWidget *_wContent;
    QHBoxLayout *_lAnimations;
    QWidget *_wAnimations;
    QPushButton *_pbEnable;
    bool _expanded;
signals:
    void animationMoved(QObject* uiAnimation, int programId, int groupId);
public slots:
    void enable(bool on);
    
protected slots:
    void _moveAnimation(int programId, int groupId);
};

#endif // UIANIMATIONGROUP_H
