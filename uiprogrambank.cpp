#include "uiprogrambank.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include "uiprogram.h"

UiProgramBank::UiProgramBank(Minotor *minotor, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *lMaster = new QVBoxLayout(this);
    lMaster->setSpacing(0);
    lMaster->setMargin(0);
    lMaster->setContentsMargins(0,0,0,0);
    QWidget *wBackground = new QWidget(this);
    wBackground->setObjectName("panel");
    lMaster->addWidget(wBackground);

    // Master
    QVBoxLayout *lBackground = new QVBoxLayout(wBackground);
    lBackground->setSpacing(0);
    lBackground->setMargin(0);
    lBackground->setContentsMargins(0,0,0,0);
    //Title
    QLabel *tTitle = new QLabel(this);
    tTitle->setObjectName("title");
    tTitle->setContentsMargins(5,5,0,0);

    lBackground->addWidget(tTitle);
    tTitle->setText("Program Bank");

    //Content
    QWidget *wContent = new QWidget(this);
    lBackground->addWidget(wContent);
    QHBoxLayout *lContent = new QHBoxLayout(wContent);
    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(5,0,5,5);
    QScrollArea *sa = new QScrollArea(wContent);
    //sa->setObjectName("scroll");
    sa->setFrameShadow(QFrame::Plain);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setWidgetResizable(true);
    lContent->addWidget(sa);

    QWidget *wScrollContent = new QWidget(sa);

    wScrollContent->setObjectName("scrollbackground");
    sa->setWidget(wScrollContent);
    QVBoxLayout *lScrollContent =  new QVBoxLayout(wScrollContent);
    _bgOnAir = new QButtonGroup(wScrollContent);

    foreach(MinoProgram* program, minotor->programs())
    {

        UiProgram *uip = new UiProgram(program, wScrollContent);
        QPushButton *pbOnAir = uip->findChild<QPushButton*>("bOnAir");
        _bgOnAir->addButton(pbOnAir);
        //connect(uip->b,SIGNAL(clicked(bool)),;
        //connect(uip, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
        lScrollContent->addWidget(uip);

    }
    lScrollContent->addStretch();

}
