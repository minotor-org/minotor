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
        lScrollContent->addWidget(uip);

    }
    lScrollContent->addStretch();
}
