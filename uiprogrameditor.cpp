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
        int programId;
        dataStream
                >> className
                >> offset
                >> programId
                >> srcId;
        qDebug() << "dragMoveEvent" << className << srcId << "(program id:" << programId << ")";
        if (programId == _program->id()) {
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
        int srcId;
        int srcProgramId;
        int srcGroupId;
        dataStream
                >> className
                >> offset
                >> srcProgramId
                >> srcGroupId
                >> srcId;


        QList<UiAnimationGroup*> uiGroups = _wContent->findChildren<UiAnimationGroup*>();

        for (int i=0;i<uiGroups.count();i++)
        {
            UiAnimationGroup *uiGroup = uiGroups.at(i);
            if (uiGroup->geometry().contains(event->pos()))
            {
                const int destGroupId = _program->animationGroups().indexOf(uiGroup->group());
                if (destGroupId == srcGroupId)
                {
                    QList<UiAnimation*> uiAnimations = uiGroup->findChildren<UiAnimation*>();
                    for (int j=0;j<uiAnimations.count();j++)
                    {
                        if(uiAnimations.at(j)->geometry().contains(event->pos()))
                        {
                            const int destId = uiGroup->group()->animations().indexOf(uiAnimations.at(j)->animation());

                            if (destId != -1)
                            {
                                uiGroup->group()->moveAnimation(srcId, destId);
                                qDebug() << "plop";
                                QLayoutItem *li = _lContent->takeAt(srcId);

                                qDebug() << "plop li";
                                if(li->widget())
                                {
                                    UiAnimation *uiAnimation = dynamic_cast<UiAnimation*>(li->widget());
                                    if(uiAnimation)
                                    {
                                        qDebug() << "plop uiA";
                                        _lContent->insertWidget(destId, uiAnimation);
                                    }
                                }
                            }
                            break;
                        }
                    }

                }
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

