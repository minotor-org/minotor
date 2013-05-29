#include "uiprogrambank.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QDebug>

#include "uiprogram.h"

UiProgramBank::UiProgramBank(MinoProgramBank *bank, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *lMaster = new QVBoxLayout(this);
    lMaster->setSpacing(0);
    lMaster->setMargin(0);
    lMaster->setContentsMargins(0,0,0,0);

    QWidget *wBorder = new QWidget(this);
    lMaster->addWidget(wBorder);
    wBorder->setObjectName("panel");

    QVBoxLayout *lBorder = new QVBoxLayout(wBorder);
    lBorder->setSpacing(0);
    lBorder->setMargin(0);
    lBorder->setContentsMargins(2,2,2,2);

    QWidget *wBackground = new QWidget(this);
    lBorder->addWidget(wBackground);
    wBackground->setObjectName("panelcontent");

    // Master
    QVBoxLayout *lBackground = new QVBoxLayout(wBackground);
    lBackground->setSpacing(0);
    lBackground->setMargin(0);
    lBackground->setContentsMargins(10,5,10,10);

    QLabel *tTitle = new QLabel(this);
    tTitle->setObjectName("title");
    tTitle->setContentsMargins(0,0,0,2);

    lBackground->addWidget(tTitle);
    tTitle->setText("Program Bank");

    QWidget *wContainer = new QWidget(this);
    lBackground->addWidget(wContainer);
    wContainer->setObjectName("scroll");
    QVBoxLayout *lContainer = new QVBoxLayout(wContainer);
    lContainer->setSpacing(0);
    lContainer->setMargin(0);
    lContainer->setContentsMargins(2,2,2,2);

    //Content
    QWidget *wContent = new QWidget(wContainer);
    lContainer->addWidget(wContent);
    wContent->setObjectName("scrollcontent");
    QHBoxLayout *lContent = new QHBoxLayout(wContent);
    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(5,5,5,5);
    QScrollArea *sa = new QScrollArea(wContent);
    sa->setFrameShadow(QFrame::Plain);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setWidgetResizable(true);
    lContent->addWidget(sa);

    _wScrollContent = new QWidget(sa);
    _wScrollContent->setObjectName("scrollbackground");
    sa->setWidget(_wScrollContent);
    QVBoxLayout *_lScrollContent =  new QVBoxLayout(_wScrollContent);
    _bgOnAir = new QButtonGroup(_wScrollContent);

    _lScrollContent->addStretch();
    foreach(MinoProgram* program, bank->programs())
    {
        addProgram(program);
    }
    connect(bank, SIGNAL(programAdded(QObject*)), this, SLOT(addProgram(QObject*)));
    connect(bank, SIGNAL(destroyed()), this, SLOT(deleteLater()));

    connect(bank, SIGNAL(programSelectorChanged(QObject*)), this, SLOT(highlightProgram(QObject*)));
}

void UiProgramBank::addProgram(MinoProgram *program)
{
    UiProgram *uip = new UiProgram(program, _wScrollContent);
    QPushButton *pbOnAir = uip->findChild<QPushButton*>("bOnAir");
    _bgOnAir->addButton(pbOnAir);
    QBoxLayout * bl = dynamic_cast<QBoxLayout*>(_wScrollContent->layout());
    Q_ASSERT(bl);
    bl->insertWidget(bl->count()-1,uip);
}

void UiProgramBank::addProgram(QObject *program)
{
    qDebug() << Q_FUNC_INFO;
    MinoProgram *p = qobject_cast<MinoProgram*>(program);
    Q_ASSERT(p);
    addProgram(p);
}

void UiProgramBank::requestAnimationGroupMove(MinoAnimationGroup *srcGroup, MinoProgram *destProgram, int destGroupId)
{
    qDebug() << Q_FUNC_INFO
             << "srcGroup:" << srcGroup
             << "destProgram:" << destProgram
             << "destGroupId" << destGroupId;
    MinoProgram *srcProgram = srcGroup->program();
    Q_ASSERT(srcProgram);
    Q_ASSERT(destProgram);
    srcProgram->moveAnimationGroup(srcGroup->id(), destGroupId, destProgram);
}

void UiProgramBank::highlightProgram(QObject *program)
{
    MinoProgram *mp = NULL;
    if(program)
    {
        mp = qobject_cast<MinoProgram*>(program);
        Q_ASSERT(mp);
    }
    QList<UiProgram*> uiPrograms = findChildren<UiProgram*>();
    foreach (UiProgram *uip, uiPrograms) {
        if(uip->program() == mp)
        {
            uip->setHighlight(true);
            const int top = uip->pos().y();
            const int half = uip->height() / 2;
            findChild<QScrollArea *>()->ensureVisible(0, top + half, 0, half);
        }
        else
        {
            uip->setHighlight(false);
        }
    }
}
