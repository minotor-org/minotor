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
    _lContent->addStretch(1);

    foreach (MinoAnimationGroup *group, _program->animationGroups())
    {
        addAnimationGroup(group);
    }

    connect (_program, SIGNAL(animationGroupAdded(QObject*)), this, SLOT(addAnimationGroup(QObject*)));

    setAcceptDrops(true);
}

UiAnimationGroup* UiProgramEditor::addAnimationGroup(MinoAnimationGroup *group)
{
    UiAnimationGroup *uiAnimationGroup = new UiAnimationGroup(group, _wContent);
    uiAnimationGroup->setExpanded(_expanded);

    _lContent->insertWidget(_lContent->count()-1, uiAnimationGroup);
    return uiAnimationGroup;
}

void UiProgramEditor::addAnimationGroup(QObject *group)
{
    qDebug() << Q_FUNC_INFO;
    MinoAnimationGroup *g = qobject_cast<MinoAnimationGroup*>(group);
    Q_ASSERT(g);
    addAnimationGroup(g);
}

UiProgramEditor::~UiProgramEditor()
{
}

void UiProgramEditor::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
    if (event->mimeData()->hasFormat("application/x-dndanimationdescription")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasFormat("application/x-dndanimation")) {
        qDebug() << Q_FUNC_INFO
                 << event->source()->metaObject()->className() << event->source()->objectName();
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
        qDebug() << Q_FUNC_INFO
                 << className << srcId << "(program id:" << programId << ")";
        event->accept();
    } else if (event->mimeData()->hasFormat("application/x-dndanimationgroup")) {
        qDebug() << Q_FUNC_INFO
                 << event->source()->metaObject()->className() << event->source()->objectName();
        QByteArray itemData = event->mimeData()->data("application/x-dndanimationgroup");
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
        qDebug() << Q_FUNC_INFO
                 << className << srcId << "(program id:" << programId << ")";

        event->accept();
    } else {
        event->ignore();
        qDebug() << Q_FUNC_INFO
                 << "ERROR: This should not happends !!!";
    }
    event->accept();
}

void UiProgramEditor::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dndanimationdescription")) {
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
        }
    } else if (event->mimeData()->hasFormat("application/x-dndanimationgroup")) {
        QByteArray itemData = event->mimeData()->data("application/x-dndanimationgroup");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString className;
        QPoint offset;
        int srcProgramId;
        int srcGroupId;

        dataStream
                >> className
                >> offset
                >> srcProgramId
                >> srcGroupId;
        if (srcProgramId == _program->id()) {
            event->setDropAction(Qt::MoveAction);
        }
    }
    event->accept();
}

void UiProgramEditor::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dndanimationdescription")) {
        QByteArray itemData = event->mimeData()->data("application/x-dndanimationdescription");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString className;
        QPoint offset;
        dataStream >> className >> offset;

        bool destGroupFound = false;
        bool destAnimationFound = false;
        QList<UiAnimationGroup*> uiGroups = _wContent->findChildren<UiAnimationGroup*>();
        for (int i=0;i<uiGroups.count();i++)
        {
            UiAnimationGroup *uiGroup = uiGroups.at(i);
            if (uiGroup->geometry().contains(event->pos()))
            {
                MinoAnimationGroup *group = uiGroup->group();
                QList<UiAnimation*> uiAnimations = uiGroup->findChildren<UiAnimation*>();
                for (int j=0; j<uiAnimations.count(); j++)
                {
                    QPoint pa = uiAnimations.at(j)->mapFrom(this, event->pos());
                    if(uiAnimations.at(j)->rect().contains(pa))
                    {
                        const int destAnimationId = group->animations().indexOf(uiAnimations.at(j)->animation());
                        if (destAnimationId != -1)
                        {
                            destAnimationFound = true;
                            MinoAnimation *animation = group->addAnimation(className,destAnimationId);
                            uiGroup->addAnimation(animation,destAnimationId);
                        }
                    }
                }
                destGroupFound = true;

                if(!destAnimationFound)
                {
                    // Group was found but no target animation under mouse: let's create it at the end of group
                    MinoAnimation *animation = group->addAnimation(className);
                    uiGroup->addAnimation(animation, -1);
                }
                break;
            }
        }

        if(!destGroupFound)
        {
            // No group found under mouse: let's create a new group
            MinoAnimationGroup *group = new MinoAnimationGroup(_program);
            MinoAnimation *animation = group->addAnimation(className);
            _program->addAnimationGroup(group);
            if(animation)
            {
                this->addAnimationGroup(group);
            }
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


        qDebug() << Q_FUNC_INFO
                 << " className:" << className
                 << " srcProgramId:" << srcProgramId
                 << " srcGroupId:" << srcGroupId
                 << " srcAnimationId:" << srcAnimationId;

        bool destGroupFound = false;
        bool destAnimationFound = false;
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
                            destAnimationFound = true;
                            moveAnimation(srcGroupId, srcAnimationId, uiGroup, destAnimationId);
                        }
                    }
                }
                destGroupFound = true;

                if (!destAnimationFound)
                {
                    moveAnimation(srcGroupId, srcAnimationId, uiGroup, -1);
                }
                break;
            }
        }

        if(!destGroupFound)
        {
            MinoAnimationGroup *group = new MinoAnimationGroup(_program);
            _program->addAnimationGroup(group);
            UiAnimationGroup *uiGroup = addAnimationGroup(group);
            moveAnimation(srcGroupId, srcAnimationId, uiGroup, -1);
        }

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasFormat("application/x-dndanimationgroup")) {
        QByteArray itemData = event->mimeData()->data("application/x-dndanimationgroup");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString className;
        QPoint offset;
        int srcProgramId;
        int srcGroupId;
        dataStream
                >> className
                >> offset
                >> srcProgramId
                >> srcGroupId;

        qDebug() << Q_FUNC_INFO
                 << " className:" << className
                 << " srcProgramId:" << srcProgramId
                 << " srcGroupId:" << srcGroupId;

        bool destGroupFound = false;
        QList<UiAnimationGroup*> uiGroups = _wContent->findChildren<UiAnimationGroup*>();
        for (int i=0;i<uiGroups.count();i++)
        {
            UiAnimationGroup *uiGroup = uiGroups.at(i);
            if (uiGroup->geometry().contains(event->pos()))
            {
                const int destGroupId = uiGroup->group()->program()->animationGroups().indexOf(uiGroup->group());
                if (destGroupId != -1)
                {
                    destGroupFound = true;
                    //move group to dest pos
                    moveAnimationGroup(srcGroupId, destGroupId);
                }
                break;
            }
        }
        if (!destGroupFound && (srcGroupId != _program->animationGroups().count()-1))
        {
            //move group at the end
            moveAnimationGroup(srcGroupId, -1);
        }
        /*
        if(!destGroupFound)
        {
            MinoAnimationGroup *group = new MinoAnimationGroup(_program);
            _program->addAnimationGroup(group);
            UiAnimationGroup *uiGroup = addAnimationGroup(group);
            moveAnimation(srcGroupId, srcAnimationId, uiGroup, -1);
        }
        */
    }
    event->accept();
}

void UiProgramEditor::setExpanded(bool expanded)
{
    _expanded = expanded;
    QList<UiAnimationGroup*> uiGroups = _wContent->findChildren<UiAnimationGroup*>();
    foreach(UiAnimationGroup *uiGroup, uiGroups)
    {
        uiGroup->setExpanded(expanded);
    }
}

UiAnimationGroup* UiProgramEditor::findUiAnimationGroup(int groupId)
{
    UiAnimationGroup *uiAnimationGroup = NULL;

    qDebug() << Q_FUNC_INFO
             << "groupId:" << groupId;
    QList<UiAnimationGroup*> uiAnimationGroups = this->findChildren<UiAnimationGroup*>();

    for (int j=0; j<uiAnimationGroups.count(); j++)
    {
        int currentGroupId = uiAnimationGroups.at(j)->group()->id();
        if(currentGroupId == groupId)
        {
            uiAnimationGroup = uiAnimationGroups.at(j);
            break;
        }
    }
    qDebug() << Q_FUNC_INFO
             << "found:" << uiAnimationGroup;
    return uiAnimationGroup;
}

UiAnimationGroup* UiProgramEditor::takeAnimationGroupAt(int groupId)
{
    UiAnimationGroup *ret = NULL;
    QList<UiAnimationGroup*> uiAnimationGroups = this->findChildren<UiAnimationGroup*>();
    if(groupId == -1)
    {
        groupId=uiAnimationGroups.count()-1;
    }
    if(uiAnimationGroups.count()>0)
    {
        qDebug() << Q_FUNC_INFO
                 << "program has" << uiAnimationGroups.count() << "items";
        for (int j=0; j<uiAnimationGroups.count(); j++)
        {
            if(uiAnimationGroups.at(j)->group()->id() == groupId)
            {
                ret = uiAnimationGroups.takeAt(j);
                break;
            }
        }
    }
    return ret;
}

void UiProgramEditor::insertAnimationGroup(UiAnimationGroup *uiAnimationGroup, int destGroupId)
{
    if (destGroupId == -1)
    {
        //Place is at the end of the list (-1 beacause there is a spacer at the end)
        destGroupId =  _lContent->count()-2;
    }
    qDebug() << Q_FUNC_INFO << "index:" << destGroupId << "grp:" << uiAnimationGroup->group();
    _lContent->insertWidget(destGroupId, uiAnimationGroup);
}

void UiProgramEditor::moveAnimation(int srcGroupId, int srcAnimationId, MinoAnimationGroup *destGroup, int destAnimationId)
{
    qDebug() << Q_FUNC_INFO
             << "srcGroupId:" << srcGroupId
             << "srcAnimationId:" << srcAnimationId
             << "destGroup:" << destGroup
             << "destAnimationId" << destAnimationId;
    MinoAnimationGroup *srcGroup = _program->animationGroups().at(srcGroupId);
    Q_ASSERT(srcGroup);
    Q_ASSERT(destGroup);
    srcGroup->moveAnimation(srcAnimationId, destAnimationId, destGroup);
}

void UiProgramEditor::moveAnimationGroup(int srcGroupId, int destGroupId)
{
    _program->moveAnimationGroup(srcGroupId, destGroupId);
}
