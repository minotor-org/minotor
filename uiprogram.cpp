#include "uiprogram.h"
#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <QCheckBox>
#include "uiprogramview.h"
#include "uiprogrameditor.h"

UiProgram::UiProgram(MinoProgram *program, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *lProgram = new QVBoxLayout(this);

    QWidget *wBackground = new QWidget(this);
    lProgram->addWidget(wBackground);
    QHBoxLayout *lBackground = new QHBoxLayout(wBackground);

    QWidget *wTitle = new QWidget(wBackground);
    lBackground->addWidget(wTitle);
    QVBoxLayout *lTitle = new QVBoxLayout(wTitle);

    QLabel *tTitle = new QLabel(wTitle);
    lTitle->addWidget(tTitle);
    tTitle->setObjectName("title");
    tTitle->setText(QString("Prg ") + QString::number(program->id()));
    lTitle->addStretch();
    UiProgramEditor * editor = new UiProgramEditor(program, wBackground);
    connect(editor, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    lBackground->addWidget(editor);

    QWidget *wRightArea = new QWidget(wBackground);
    lBackground->addWidget(wRightArea);
    wRightArea->setMaximumWidth(210);
    QVBoxLayout *lRightArea = new QVBoxLayout(wRightArea);

    QWidget *wCollapse = new QWidget(wRightArea);
    lRightArea->addWidget(wCollapse);
    QHBoxLayout *lCollapse = new QHBoxLayout(wCollapse);
    lCollapse->addStretch();
    QCheckBox *cbCollapse = new QCheckBox(wCollapse);
    lCollapse->addWidget(cbCollapse);
    // Monitor
    QFrame *fPreview = new QFrame(this);
    lRightArea->addWidget(fPreview);
    QVBoxLayout *lPreview = new QVBoxLayout(fPreview);
    fPreview->setObjectName("view");
    fPreview->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    fPreview->setMinimumSize(200, 150);
    UiProgramView *pView = new UiProgramView(program, fPreview);
    QSizePolicy policy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    lPreview->addWidget(pView);
    lRightArea->addStretch();



}
