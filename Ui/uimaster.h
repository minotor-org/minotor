#ifndef UIMASTER_H
#define UIMASTER_H

#include <QWidget>

#include "minomaster.h"
#include "uiprogramview.h"
#include "uianimationgroup.h"
#include "uianimation.h"
#include "uimastercontrol.h"

class UiMaster : public QWidget
{
    Q_OBJECT
public:
    explicit UiMaster(MinoMaster *master, QWidget *parent);
    UiAnimation* takeAnimationAt(int groupId, int animationId);
    UiAnimationGroup* findUiAnimationGroup(int groupId);
    void insertAnimation(UiAnimation *uiAnimation, int destGroupId, int destAnimationId);
signals:
    void midiLearnToggled(bool);
public slots:
    void updateProgram();
protected:
    MinoMaster *_master;
private:
    UiProgramView *_uiMasterMonitor;
    UiMasterControl *_uiMasterControl;
private slots:

};

#endif // UIMASTER_H
