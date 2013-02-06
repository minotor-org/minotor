#include "uiprogrambank.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "uiprogram.h"

UiProgramBank::UiProgramBank(Minotor *minotor, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *lMaster = new QVBoxLayout(this);
    QWidget *wBackground = new QWidget(this);
    wBackground->setObjectName("panel");
    lMaster->addWidget(wBackground);

    // Master
    QVBoxLayout *lBackground = new QVBoxLayout(wBackground);

    //Title
    QLabel *tTitle = new QLabel(this);
    tTitle->setObjectName("title");
    lBackground->addWidget(tTitle);
    tTitle->setText("Program Bank");

    //Content
    QWidget *wContent = new QWidget(this);
    lBackground->addWidget(wContent);
    QHBoxLayout *lContent = new QHBoxLayout(wContent);

    QScrollArea *sa = new QScrollArea(wContent);
    sa->setObjectName("scroll");
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

    foreach(MinoProgram* program, minotor->programs())
    {

        UiProgram *uip = new UiProgram(program, wScrollContent);
        //connect(uip, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
        lScrollContent->addWidget(uip);

    }
    lScrollContent->addStretch();

}
