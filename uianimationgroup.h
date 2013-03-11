#ifndef UIANIMATIONGROUP_H
#define UIANIMATIONGROUP_H

#include <QGroupBox>
#include <QCheckBox>
#include <QLayout>

#include "minoanimationgroup.h"
#include "minoanimation.h"

class UiAnimationGroup : public QGroupBox
{
    Q_OBJECT
public:
    explicit UiAnimationGroup(MinoAnimationGroup *group, QWidget *parent);
    void setExpanded(bool on);
    void addAnimation(MinoAnimation *animation);

    // Accessors
    MinoAnimationGroup* group() const { return _group; }

private:
    MinoAnimationGroup *_group;
    QWidget *_wEnable;
    QHBoxLayout *_lContent;
    QWidget *_wContent;
    QCheckBox *_cbEnable;
    bool _expanded;
signals:
    
public slots:
    void enable(bool on);
    
};

#endif // UIANIMATIONGROUP_H
