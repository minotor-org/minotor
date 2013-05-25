#ifndef UIMASTERCONTROL_H
#define UIMASTERCONTROL_H

#include <QWidget>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QScrollArea>

#include "minomaster.h"
#include "uimasteranimationgroup.h"

class UiMasterControl : public QWidget
{
    Q_OBJECT
public:
    explicit UiMasterControl(MinoMaster *master, QWidget *parent);
    ~UiMasterControl();

protected:
    QWidget *_wContent;

    void addAnimationGroup(MinoAnimationGroup *group);
    UiMasterAnimationGroup* takeAnimationGroupAt(int groupId);
    void insertAnimationGroup(UiMasterAnimationGroup *uiMasterAnimationGroup, int destGroupId);

signals:
    
public slots:
    void updateProgram();

private slots:
    void animationMoved(int srcProgramId, int srcGroupId , int srcAnimationId, int destProgramId, int destGroupId , int destAnimationId);
    void animationGroupMoved(int srcProgramId, int srcGroupId, int destGroupId);
    void changeViewportRange(const int min, const int max);

private:
    MinoMaster *_master;
    int _viewportMin;
    int _viewportMax;
    QScrollArea *_sa;
};

#endif // UIMASTERCONTROL_H
