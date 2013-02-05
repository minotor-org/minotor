#include "uiprogrameditor.h"
#include "uianimation.h"

#include "minoanimation.h"
#include "uiprogramview.h"

#include <QFrame>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

UiProgramEditor::UiProgramEditor(MinoProgram *program, QWidget *parent) :
    QWidget(parent),
    _program(program)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Animations properties
    QScrollArea *sa = new QScrollArea(this);
    layout->addWidget(sa);

    _wContent = new QWidget(this);
    sa->setWidget(_wContent);
    sa->setWidgetResizable(true);

    QVBoxLayout *lContent = new QVBoxLayout(_wContent);
    lContent->addStretch();

    foreach (MinoAnimation *animation, _program->animations())
    {
        addAnimation(animation);
    }

    setAcceptDrops(true);
}

void UiProgramEditor::addAnimation(MinoAnimation *animation)
{
    UiAnimation *uiAnimation = new UiAnimation(animation, _wContent);
    connect(uiAnimation, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));

    dynamic_cast<QBoxLayout*>(_wContent->layout())->insertWidget(_wContent->layout()->count()-1, uiAnimation);
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

        qDebug() << "UiChannelEditor::dropEvent"
                 << "x-dndanimationdescrition" << className;

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
    } else {
        event->ignore();
    }
}
