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

    QWidget *wContent = new QWidget(this);
    sa->setWidget(wContent);
    sa->setWidgetResizable(true);

    QVBoxLayout *lContent = new QVBoxLayout(wContent);
    foreach (MinoAnimation *animation, _channel->animations())
    {
        UiAnimation *uiAnimation = new UiAnimation(animation, wContent);
        connect(uiAnimation, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));

        lContent->addWidget(uiAnimation);

        QFrame *frame = new QFrame(wContent);
        frame->setFrameShape(QFrame::HLine);
        lContent->addWidget(frame);
    }
    lContent->addStretch();
    //layout->addStretch();
}

UiChannelEditor::~UiChannelEditor()
{

}
