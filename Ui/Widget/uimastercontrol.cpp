#include "uimastercontrol.h"
#include "uianimation.h"

#include "minoanimation.h"
#include "uimasteranimationgroup.h"
#include "uiprogramview.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

UiMasterControl::UiMasterControl(MinoMaster *master, QWidget *parent) :
    QWidget(parent),
    _master(master),
    _viewportMin(-1),
    _viewportMax(-1)
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
    _sa = new QScrollArea(wContent);
    lContent->addWidget(_sa);

    _wContent = new QWidget(this);
    _wContent->setObjectName("scrollbackground");
    _sa->setWidget(_wContent);
    _sa->setFrameShadow(QFrame::Plain);
    _sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    _sa->setFocusPolicy(Qt::NoFocus);
    _sa->setWidgetResizable(true);

    QHBoxLayout *_lContent = new QHBoxLayout(_wContent);
    _lContent->addStretch(1);

    updateProgram();

    connect(_master,SIGNAL(programChanged()),this,SLOT(updateProgram()));
    connect(_master,SIGNAL(updated()),this,SLOT(updateProgram()));
    connect(_master, SIGNAL(viewportRangeChanged(int,int)), this, SLOT(changeViewportRange(int,int)));
}

void UiMasterControl::addAnimationGroup(MinoAnimationGroup *group)
{
    UiMasterAnimationGroup *uiAnimationGroup = new UiMasterAnimationGroup(group, _wContent);
    const int position = _wContent->layout()->count()-1;
    if ((position >= _viewportMin) && (position <= _viewportMax)) {
        uiAnimationGroup->setHighlight(true);
    }
    dynamic_cast<QBoxLayout*>(_wContent->layout())->insertWidget(position, uiAnimationGroup);
}

void UiMasterControl::updateProgram()
{
    foreach (UiMasterAnimationGroup *group, this->findChildren<UiMasterAnimationGroup*>())
    {
        delete(group);
    }
    if(_master->program())
    {
        foreach (MinoAnimationGroup *group, _master->program()->animationGroups())
        {
            addAnimationGroup(group);
        }
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
    if ((destGroupId >= _viewportMin) && (destGroupId <= _viewportMax)) {
        uiMasterAnimationGroup->setHighlight(true);
    } else {
        uiMasterAnimationGroup->setHighlight(false);
    }
    dynamic_cast<QBoxLayout*>(_wContent->layout())->insertWidget(destGroupId, uiMasterAnimationGroup);
}

void UiMasterControl::changeViewportRange(const int min, const int max)
{
    if((_viewportMin!=min) || (_viewportMax!=max))
    {
        _viewportMin = min;
        _viewportMax = max;

        int left = -1;
        int right = -1;
        foreach (UiMasterAnimationGroup *group, this->findChildren<UiMasterAnimationGroup*>())
        {
            const int position = group->group()->id();
            if ((position >= _viewportMin) && (position <= _viewportMax)) {
                group->setHighlight(true);
                if(left==-1)
                {
                    left = group->pos().x();
                } else {
                    left = qMin(left, group->pos().x());
                }

                right = qMax(right, group->pos().x() + group->width());
            } else {
                group->setHighlight(false);
            }
        }
        const int half = (right - left) / 2;
        const int middle = left + half;
        qDebug() << Q_FUNC_INFO
                    << "left" << left
                    << "right" << right
                    << "half" << half
                       << "middle" << middle;
        _sa->ensureVisible(middle, 0, half);
    }
}
