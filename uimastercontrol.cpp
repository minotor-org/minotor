#include "uimastercontrol.h"
#include "uianimation.h"

#include "minoanimation.h"
#include "uimasteranimationgroup.h"
#include "uiprogramview.h"

#include <QFrame>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

UiMasterControl::UiMasterControl(MinoMaster *master, QWidget *parent) :
    QWidget(parent),
    _master(master)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,5);

    QWidget *wContainer = new QWidget(this);
    layout->addWidget(wContainer);
    wContainer->setObjectName("scroll");
    QVBoxLayout *lContainer = new QVBoxLayout(wContainer);
    lContainer->setSpacing(0);
    lContainer->setMargin(0);
    lContainer->setContentsMargins(2,2,2,2);

    QWidget *wContent = new QWidget(wContainer);
    lContainer->addWidget(wContent);
    wContent->setObjectName("scrollcontent");
    QVBoxLayout *lContent = new QVBoxLayout(wContent);

    // Animations properties
    QScrollArea *sa = new QScrollArea(wContent);
    lContent->addWidget(sa);


    _wContent = new QWidget(this);
    _wContent->setObjectName("scrollbackground");
    sa->setWidget(_wContent);
    sa->setFrameShadow(QFrame::Plain);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setWidgetResizable(true);

    QHBoxLayout *_lContent = new QHBoxLayout(_wContent);
    _lContent->addStretch(1);

    updateProgram();

    connect(_master,SIGNAL(programChanged()),this,SLOT(updateProgram()));
    connect(_master,SIGNAL(updated()),this,SLOT(updateProgram()));
}

void UiMasterControl::addAnimationGroup(MinoAnimationGroup *group)
{
    UiMasterAnimationGroup *uiAnimationGroup = new UiMasterAnimationGroup(group, _wContent);
    dynamic_cast<QBoxLayout*>(_wContent->layout())->insertWidget(_wContent->layout()->count()-1, uiAnimationGroup);
}

void UiMasterControl::updateProgram()
{
    foreach (UiMasterAnimationGroup *group, this->findChildren<UiMasterAnimationGroup*>())
    {
        delete(group);
    }
    foreach (MinoAnimationGroup *group, _master->program()->animationGroups())
    {
        addAnimationGroup(group);
    }
}

UiMasterControl::~UiMasterControl()
{

}

void UiMasterControl::animationMoved(int srcProgramId, int srcGroupId , int srcAnimationId, int destProgramId, int destGroupId , int destAnimationId)
{
    //FIXME remove useless variables
    (void)srcProgramId;
    (void)srcAnimationId;
    (void)destProgramId;
    (void)destAnimationId;

    foreach (UiMasterAnimationGroup *group, this->findChildren<UiMasterAnimationGroup*>())
    {
        if (srcGroupId == group->group()->id() || destGroupId == group->group()->id())
        {
            group->updateGroup();
            qDebug() << Q_FUNC_INFO
                     << "update group" << group->group()->id();
        }
    }
}

void UiMasterControl::animationGroupMoved(int srcProgramId, int srcGroupId, int destGroupId)
{
    //FIXME remove useless variables
    (void)srcGroupId;

    if (srcProgramId == _master->program()->id())
    {
        UiMasterAnimationGroup *uiMasterGroup = takeAnimationGroupAt(destGroupId);
        insertAnimationGroup(uiMasterGroup, destGroupId);
    }
}

UiMasterAnimationGroup* UiMasterControl::takeAnimationGroupAt(int groupId)
{
    UiMasterAnimationGroup *ret = NULL;
    QList<UiMasterAnimationGroup*> uiMasterAnimationGroup = this->findChildren<UiMasterAnimationGroup*>();
    if(groupId == -1)
    {
        groupId=uiMasterAnimationGroup.count()-1;
    }
    if(uiMasterAnimationGroup.count()>0)
    {
        qDebug() << Q_FUNC_INFO
                 << "program has" << uiMasterAnimationGroup.count() << "items";
        for (int j=0; j<uiMasterAnimationGroup.count(); j++)
        {
            if(uiMasterAnimationGroup.at(j)->group()->id() == groupId)
            {
                ret = uiMasterAnimationGroup.takeAt(j);
                break;
            }
        }
    }
    return ret;
}

void UiMasterControl::insertAnimationGroup(UiMasterAnimationGroup *uiMasterAnimationGroup, int destGroupId)
{
    if (destGroupId == -1)
    {
        //Place is at the end of the list (-1 beacause there is a spacer at the end)
        destGroupId =  _wContent->layout()->count()-2;
    }
    dynamic_cast<QBoxLayout*>(_wContent->layout())->insertWidget(destGroupId, uiMasterAnimationGroup);
}
