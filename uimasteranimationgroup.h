#ifndef UIMASTERANIMATIONGROUP_H
#define UIMASTERANIMATIONGROUP_H

#include <QGroupBox>
#include <QPushButton>
#include <QLayout>

#include "minoanimationgroup.h"
#include "minoanimation.h"
#include "uianimation.h"

class UiMasterAnimationGroup : public QGroupBox
{
    Q_OBJECT
public:
    explicit UiMasterAnimationGroup(MinoAnimationGroup *group, QWidget *parent);
    ~UiMasterAnimationGroup();

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
    
public slots:
    void enable(bool on);
    
};

#endif // UIMASTERANIMATIONGROUP_H