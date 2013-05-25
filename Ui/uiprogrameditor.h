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
    UiAnimationGroup* takeAnimationGroupAt(int groupId);
    void insertUiAnimationGroup(UiAnimationGroup *uiAnimationGroup, int destGroupId);

    template<typename T> T findParent();
    template<typename T> T findContainer(QWidget* parent, const QPoint pos);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    bool dropMinoAnimationDescription(QDropEvent *event);
    bool dropMinoAnimation(QDropEvent *event);
    bool dropMinoAnimationGroup(QDropEvent *event);

    QWidget *_wContent;
    QBoxLayout *_lContent;

    UiAnimationGroup* addAnimationGroup(MinoAnimationGroup *group);
    void requestAnimationMove(MinoAnimation *srcAnimation, MinoAnimationGroup *destGroup, int destAnimationId);

signals:

public slots:
    void setExpanded(bool expanded);

private slots:
    void addAnimationGroup(QObject* group);
    void moveAnimationGroup(QObject *group);

private:
    MinoProgram *_program;
    bool _expanded;
};

#endif // UICHANNELEDITOR_H
