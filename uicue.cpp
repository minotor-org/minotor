#include "uicue.h"

#include <QFrame>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "uiprogramview.h"

UiCue::UiCue(MinoProgram *program, QWidget *parent) :
    QWidget(parent),
    _program(program)
{
    // Cue
    QVBoxLayout *lCueViewAndControls = new QVBoxLayout(this);

    // Controls
    QWidget *wControls = new QWidget(this);
    lCueViewAndControls->addWidget(wControls);
    QHBoxLayout *lControls = new QHBoxLayout(wControls);
    lControls->addWidget(new QPushButton("Send"));
    lControls->addWidget(new QPushButton("Swap"));

    // View
    QFrame *fCueView = new QFrame(this);
    fCueView->setFrameShape(QFrame::Box);
    fCueView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    fCueView->setMinimumSize(250, 170);
    lCueViewAndControls->addWidget(fCueView);

    QVBoxLayout *lCueView = new QVBoxLayout(fCueView);
    UiProgramView *uiCueView = new UiProgramView(program, fCueView);
    QSizePolicy policy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    uiCueView->setMinimumSize(240, 160);
    uiCueView->setSizePolicy(policy);
    lCueView->addWidget(uiCueView);
}

