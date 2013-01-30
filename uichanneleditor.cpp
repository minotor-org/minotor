#include "uichanneleditor.h"
#include "uianimation.h"

#include "minoanimation.h"
#include "uichannelview.h"

#include <QFrame>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

UiChannelEditor::UiChannelEditor(MinoChannel *channel, QWidget *parent) :
    QWidget(parent),
    _channel(channel)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Animations properties
    QScrollArea *sa = new QScrollArea();
    layout->addWidget(sa);

    _wContent = new QWidget(this);
    sa->setWidget(_wContent);
    sa->setWidgetResizable(true);

    QVBoxLayout *lContent = new QVBoxLayout(_wContent);
    lContent->addStretch();

    foreach (MinoAnimation *animation, _channel->animations())
    {
        addAnimation(animation);
    }

    setAcceptDrops(true);
}

void UiChannelEditor::addAnimation(MinoAnimation *animation)
{
    UiAnimation *uiAnimation = new UiAnimation(animation, _wContent);
    connect(uiAnimation, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));

    dynamic_cast<QBoxLayout*>(_wContent->layout())->insertWidget(_wContent->layout()->count()-1, uiAnimation);

    QFrame *frame = new QFrame(_wContent);
    frame->setFrameShape(QFrame::HLine);
    dynamic_cast<QBoxLayout*>(_wContent->layout())->insertWidget(_wContent->layout()->count()-1, frame);
}

UiChannelEditor::~UiChannelEditor()
{

}

void UiChannelEditor::dragEnterEvent(QDragEnterEvent *event)
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

void UiChannelEditor::dragMoveEvent(QDragMoveEvent *event)
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

void UiChannelEditor::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dndanimationdescrition")) {
        QByteArray itemData = event->mimeData()->data("application/x-dndanimationdescrition");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString className;
        QPoint offset;
        dataStream >> className >> offset;

        qDebug() << className;
        MinoAnimation *animation = _channel->addAnimation(className);
        this->addAnimation(animation);

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
