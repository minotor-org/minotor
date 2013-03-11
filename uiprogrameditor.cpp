#include "uiprogrameditor.h"
#include "uianimation.h"

#include "minoanimation.h"
#include "uiprogramview.h"

#include <QFrame>
#include <QScrollArea>
#include <QLayout>
#include <QStyle>

#include <QDebug>

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

    foreach (MinoAnimation *animation, _program->animations())
    {
        addAnimation(animation);
    }

    setAcceptDrops(true);
}

void UiProgramEditor::addAnimation(MinoAnimation *animation)
{
    UiAnimation *uiAnimation = new UiAnimation(animation, _wContent);
    uiAnimation->setExpanded(_expanded);
    connect(uiAnimation, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));

    _lContent->insertWidget(_lContent->count()-1, uiAnimation);
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

        MinoAnimation *animation = _program->addAnimation(className);

        if(animation)
        {
            this->addAnimation(animation);
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
        int programId;
        dataStream
                >> className
                >> offset
                >> programId
                >> srcId;

        QList<UiAnimation*> children= _wContent->findChildren<UiAnimation*>();
        for (int i=0;i<children.count();i++)
        {
            if(children.at(i)->geometry().contains(event->pos()))
            {
                const int destId = _program->animations().indexOf(children.at(i)->animation());
                qDebug() << "UiProgramEditor::dropEvent"
                         << "x-dndanimation" << className << offset << srcId << destId;

                if (destId != -1)
                {
                    _program->moveAnimation(srcId, destId);
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

