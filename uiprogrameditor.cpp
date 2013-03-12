#include "uiprogrameditor.h"

#include "minoanimation.h"
#include "uiprogramview.h"
#include <QFrame>
#include <QScrollArea>
#include <QLayout>
#include <QStyle>

#include <QDebug>

#include "uianimation.h"
#include "uianimationgroup.h"

UiProgramEditor::UiProgramEditor(MinoProgram *program, QWidget *parent) :
    QWidget(parent),
    _program(program),
    _expanded(true)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

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
    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(3,3,3,3);
    // Animations properties
    QScrollArea *sa = new QScrollArea(this);
    lContent->addWidget(sa);

    _wContent = new QWidget(this);
    _wContent->setObjectName("scrollbackground");
    sa->setWidget(_wContent);
    sa->setFrameShadow(QFrame::Plain);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sa->setWidgetResizable(true);

    _lContent = new QHBoxLayout(_wContent);
    _lContent->addStretch();

    foreach (MinoAnimationGroup *group, _program->animationGroups())
    {
        addAnimationGroup(group);
    }

    setAcceptDrops(true);
}

void UiProgramEditor::addAnimationGroup(MinoAnimationGroup *group)
{
    UiAnimationGroup *uiAnimationGroup = new UiAnimationGroup(group, _wContent);
    uiAnimationGroup->setExpanded(_expanded);

    _lContent->insertWidget(_lContent->count()-1, uiAnimationGroup);
}

UiProgramEditor::~UiProgramEditor()
{
}

void UiProgramEditor::dragEnterEvent(QDragEnterEvent *event)
 {
     if (event->mimeData()->hasFormat("application/x-dndanimationdescrition")) {
         if (event->source() == this) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else {
             event->acceptProposedAction();
         }
     } else if (event->mimeData()->hasFormat("application/x-dndanimation")) {
         qDebug() << "dragEnterEvent" << event->source()->metaObject()->className() << event->source()->objectName();
         QByteArray itemData = event->mimeData()->data("application/x-dndanimation");
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

         QString className;
         QPoint offset;
         int srcId;
         int programId;
         dataStream
                 >> className
                 >> offset
                 >> programId
                 >> srcId;
         qDebug() << "dragEnterEvent" << className << srcId << "(program id:" << programId << ")";

         event->accept();
     } else {
         event->ignore();
     }
 }

void UiProgramEditor::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dndanimationdescrition")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasFormat("application/x-dndanimation")) {
        qDebug() << "dragMoveEvent" << event->source()->metaObject()->className() << event->source()->objectName();
        QByteArray itemData = event->mimeData()->data("application/x-dndanimation");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString className;
        QPoint offset;
        int srcId;
        int srcProgramId;
        int srcGroupId;

        dataStream
                >> className
                >> offset
                >> srcProgramId
                >> srcGroupId
                >> srcId;
        if (srcProgramId == _program->id()) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            //event->acceptProposedAction();
            event->ignore();
        }
    } else {
        event->ignore();
    }
}

void UiProgramEditor::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dndanimationdescrition")) {
        QByteArray itemData = event->mimeData()->data("application/x-dndanimationdescrition");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString className;
        QPoint offset;
        dataStream >> className >> offset;


        MinoAnimationGroup *group = new MinoAnimationGroup(_program);
        MinoAnimation *animation = group->addAnimation(className);
        _program->addAnimationGroup(group);

        if(animation)
        {
            this->addAnimationGroup(group);
        }

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasFormat("application/x-dndanimation")) {
        QByteArray itemData = event->mimeData()->data("application/x-dndanimation");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString className;
        QPoint offset;
        int srcAnimationId;
        int srcProgramId;
        int srcGroupId;
        dataStream
                >> className
                >> offset
                >> srcProgramId
                >> srcGroupId
                >> srcAnimationId;


        qDebug() << "UiProgramEditor>"
                 << " className:" << className
                 << " srcProgramId:" << srcProgramId
                 << " srcGroupId:" << srcGroupId
                 << " srcAnimationId:" << srcAnimationId;
        QList<UiAnimationGroup*> uiGroups = _wContent->findChildren<UiAnimationGroup*>();

        for (int i=0;i<uiGroups.count();i++)
        {
            UiAnimationGroup *uiGroup = uiGroups.at(i);
            if (uiGroup->geometry().contains(event->pos()))
            {
                QList<UiAnimation*> uiAnimations = uiGroup->findChildren<UiAnimation*>();
                for (int j=0; j<uiAnimations.count(); j++)
                {
                    QPoint pa = uiAnimations.at(j)->mapFrom(this, event->pos());
                    if(uiAnimations.at(j)->rect().contains(pa))
                    {
                        const int destAnimationId = uiGroup->group()->animations().indexOf(uiAnimations.at(j)->animation());
                        if (destAnimationId != -1)
                        {
                            moveAnimation(srcGroupId, srcAnimationId, uiGroup, destAnimationId);
                        }
                    }
                }
                break;
            }
        }

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }

    } else {
        event->ignore();
    }
}

void UiProgramEditor::setExpanded(bool expanded)
{
    _expanded = expanded;
    QList<UiAnimation*> animations = _wContent->findChildren<UiAnimation*>();
    foreach(UiAnimation *animation, animations)
    {
        animation->setExpanded(expanded);
    }
}

UiAnimationGroup* UiProgramEditor::findUiAnimationGroup(int groupId)
{
    UiAnimationGroup *uiAnimationGroup = NULL;

    qDebug() << "UiProgramEditor::findUiAnimationGroup(" << groupId << ")";
    QList<UiAnimationGroup*> uiAnimationGroups = this->findChildren<UiAnimationGroup*>();

    for (int j=0; j<uiAnimationGroups.count(); j++)
    {
        int currentGroupId = _program->animationGroups().indexOf(uiAnimationGroups.at(j)->group());
        if(currentGroupId == groupId)
        {
            uiAnimationGroup = uiAnimationGroups.at(j);
            break;
        }
    }
    qDebug() << "found:" << uiAnimationGroup;
    return uiAnimationGroup;
}

UiAnimation* UiProgramEditor::takeAnimationAt(int groupId, int animationId)
{
    UiAnimation *ret = NULL;
    UiAnimationGroup *uiAnimationGroup = findUiAnimationGroup(groupId);

    if(uiAnimationGroup)
    {
        QList<UiAnimation*> uiAnimations = uiAnimationGroup->findChildren<UiAnimation*>();
        qDebug() << "group have" << uiAnimations.count() << "items";
        for (int j=0; j<uiAnimations.count(); j++)
        {
            qDebug() << "animations:" << uiAnimations.at(j)->animation()->group()->animations();
            const int aId = uiAnimations.at(j)->animation()->group()->animations().indexOf(uiAnimations.at(j)->animation());
            qDebug() << "current id:" << aId << "animation id" << animationId;
            if(aId == animationId)
            {
                ret = uiAnimationGroup->takeAt(aId);
                break;
            }
        }
    }
    return ret;
}

void UiProgramEditor::insertAnimation(UiAnimation *uiAnimation, int destGroupId, int destAnimationId)
{
    UiAnimationGroup *destGroup = findUiAnimationGroup(destGroupId);
    if(destGroup)
    {
        destGroup->insertAnimation(uiAnimation, destAnimationId);
    }
}

void UiProgramEditor::moveAnimation(int srcGroupId, int srcAnimationId, UiAnimationGroup *destGroup, int destAnimationId)
{
    const int destGroupId = _program->animationGroups().indexOf(destGroup->group());
    qDebug() << "UiProgramEditor>"
             << " srcGroupId:" << srcGroupId
             << " srcAnimationId:" << srcAnimationId
             << " destGroupId:" << destGroupId
             << " destAnimationId:" << destAnimationId;

    if (destGroupId == srcGroupId)
    {
        destGroup->moveAnimation(srcAnimationId, destAnimationId);
    } else {
        UiAnimation *uiAnimation = takeAnimationAt(srcGroupId, srcAnimationId);
        qDebug() << "uiAnimation: " << uiAnimation;
        insertAnimation(uiAnimation, destGroupId, destAnimationId);
    }
    _program->moveAnimation(_program->animationGroups().at(srcGroupId), srcAnimationId, destGroup->group(), destAnimationId);
}
