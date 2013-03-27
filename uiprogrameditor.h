#ifndef UICHANNELEDITOR_H
#define UICHANNELEDITOR_H

#include <QWidget>
#include <QLayout>

#include <QDragEnterEvent>
#include <QDropEvent>

#include "minoprogram.h"
#include "minoanimationgroup.h"
#include "uianimationgroup.h"

class UiProgramEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgramEditor(MinoProgram *program, QWidget *parent);
    ~UiProgramEditor();
    UiAnimation* takeAnimationAt(int groupId, int animationId);
    UiAnimationGroup* findUiAnimationGroup(int groupId);
    void insertAnimation(UiAnimation *uiAnimation, int destGroupId, int destAnimationId);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    QWidget *_wContent;
    QBoxLayout *_lContent;

    UiAnimationGroup* addAnimationGroup(MinoAnimationGroup *group);
    void moveAnimation(int srcGroupId, int srcAnimationId, UiAnimationGroup *destGroup, int destAnimationId);
signals:

public slots:
    void setExpanded(bool expanded);
private slots:
    void animationMoved(int srcProgramId, int srcGroupId , int srcAnimationId, int destProgramId, int destGroupId , int destAnimationId);

private:
    MinoProgram *_program;
    bool _expanded;
};

#endif // UICHANNELEDITOR_H
