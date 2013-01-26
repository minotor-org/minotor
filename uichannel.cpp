#include "uichannel.h"
#include "uianimation.h"

#include "minoanimation.h"
#include "uichannelview.h"

#include <QFrame>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QDebug>

UiChannel::UiChannel(MinoChannel *channel, QWidget *parent) :
    QWidget(parent),
    _channel(channel)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Preview
    QFrame *fPreview = new QFrame(this);
    fPreview->setFrameShape(QFrame::Box);
    fPreview->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    fPreview->setMinimumSize(250, 170);
    layout->addWidget(fPreview);

    QVBoxLayout *lPreview = new QVBoxLayout(fPreview);
    UiChannelView *preview = new UiChannelView(channel, fPreview);

    QSizePolicy policy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    preview->setSizePolicy(policy);
    preview->setMinimumSize(240, 160);
    lPreview->addWidget(preview);

    // Animations properties
    QScrollArea *sa = new QScrollArea();
    layout->addWidget(sa);

    QWidget *wContent = new QWidget(this);
    sa->setWidget(wContent);
    sa->setWidgetResizable(true);

    QHBoxLayout *lContent = new QHBoxLayout(wContent);

    foreach (MinoAnimation *animation, _channel->animations())
    {
        UiAnimation *uiAnimation = new UiAnimation(animation, wContent);
        connect(uiAnimation, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));

        lContent->addWidget(uiAnimation);

        QFrame *frame = new QFrame(wContent);
        frame->setFrameShape(QFrame::VLine);
        lContent->addWidget(frame);
    }
}

UiChannel::~UiChannel()
{

}


/*
QAction * UiChannel::midiCaptureAction()
{
  return _midiCaptureAction;
}*/
