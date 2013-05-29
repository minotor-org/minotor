#include "uiprogram.h"

#include <QDebug>
#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <QCheckBox>
#include <QPushButton>
#include <QStyle>
#include <QDesktopServices>
#include <QFileDialog>

#include "minotor.h"

#include "uiprogramview.h"
#include "uiprogrameditor.h"
#include "uimidicontrollableparameter.h"
#include "uianimation.h"

UiProgram::UiProgram(MinoProgram *program, QWidget *parent) :
    QWidget(parent),
    _program(program),
    _highlight(false)
{
    QVBoxLayout *lProgram = new QVBoxLayout(this);
    lProgram->setSpacing(0);
    lProgram->setMargin(0);
    lProgram->setContentsMargins(0,0,0,0);

    _wBorder = new QWidget(this);
    _wBorder->setObjectName("panel");
    lProgram->addWidget(_wBorder);

    QVBoxLayout *lBorder = new QVBoxLayout(_wBorder);
    lBorder->setSpacing(0);
    lBorder->setMargin(0);
    lBorder->setContentsMargins(2,2,2,2);

    _wBackground = new QWidget(_wBorder);
    _wBackground->setObjectName("programbackground");

    lBorder->addWidget(_wBackground);
    QHBoxLayout *lBackground = new QHBoxLayout(_wBackground);

    QWidget *wProgramControl = new QWidget(_wBackground);
    wProgramControl->setMinimumWidth(85);
    wProgramControl->setMaximumWidth(85);
    lBackground->addWidget(wProgramControl);

    QFrame *fHighlight = new QFrame(_wBackground);
    _wHighlight = fHighlight;
    fHighlight->setObjectName("programline");
    fHighlight->setFrameShape(QFrame::VLine);
    fHighlight->setFrameShadow(QFrame::Sunken);
    fHighlight->setLineWidth(1);
    lBackground->addWidget(fHighlight);

    QVBoxLayout *lProgramControl = new QVBoxLayout(wProgramControl);
    lProgramControl->setSpacing(5);
    lProgramControl->setMargin(0);
    lProgramControl->setContentsMargins(0,0,0,0);

    QWidget *wTitle = new QWidget(wProgramControl);
    lProgramControl->addWidget(wTitle);
    QHBoxLayout *lTitle = new QHBoxLayout(wTitle);

    QLabel *tTitle = new QLabel(wTitle);
    lTitle->addWidget(tTitle);
    tTitle->setObjectName("title");
    tTitle->setText(QString("Prg ") + QString::number(program->id()));

    lTitle->addStretch();

    QPushButton *pbDelete = new QPushButton(wTitle);
    pbDelete->setFocusPolicy(Qt::NoFocus);
    pbDelete->setIcon(QIcon(":/pictos/close.png"));
    pbDelete->setIconSize(QSize(10,10));
    pbDelete->setMinimumSize(16,16);
    pbDelete->setMaximumSize(16,16);
    lTitle->addWidget(pbDelete);
    connect(pbDelete, SIGNAL(clicked(bool)),_program, SLOT(deleteLater()));

    QWidget *wBeat = new QWidget(wProgramControl);
    lProgramControl->addWidget(wBeat);
    QHBoxLayout *lBeat = new QHBoxLayout(wBeat);
    lBeat->setSpacing(0);
    lBeat->setMargin(0);
    lBeat->setContentsMargins(0,0,0,0);
    lBeat->addStretch();

    MidiControllableParameter *mpBeat = _program->findChild<MidiControllableParameter*>();
    if (mpBeat)
    {
        UiMidiControllableParameter *umpBeat = new UiMidiControllableParameter(mpBeat, this);
        lBeat->addWidget(umpBeat);
    }

    lBeat->addStretch();

    QPushButton *pbExport = new QPushButton(wProgramControl);
    pbExport->setText("Export");
    pbExport->setMinimumSize(85,16);
    pbExport->setMaximumSize(85,16);
    connect(pbExport, SIGNAL(clicked()),this, SLOT(exportProgram()));
    lProgramControl->addWidget(pbExport);
    lProgramControl->addStretch();


    UiProgramEditor * editor = new UiProgramEditor(program, _wBackground);
    lBackground->addWidget(editor);

    QWidget *wRightArea = new QWidget(_wBackground);
    lBackground->addWidget(wRightArea);
    wRightArea->setMaximumWidth(212);
    QVBoxLayout *lRightArea = new QVBoxLayout(wRightArea);

    QWidget *wCollapse = new QWidget(wRightArea);
    lRightArea->addWidget(wCollapse);
    QHBoxLayout *lCollapse = new QHBoxLayout(wCollapse);
    QPushButton *bOnAir = new QPushButton(wRightArea);
    bOnAir->setCheckable(true);
    bOnAir->setObjectName("bOnAir");
    bOnAir->setText("On Air");
    connect(bOnAir,SIGNAL(toggled(bool)),this,SLOT(requestMasterProgramChange(bool)));
    lCollapse->addWidget(bOnAir);
    lCollapse->addStretch();
    QCheckBox *cbCollapse = new QCheckBox(wCollapse);
    cbCollapse->setObjectName("arrow");
    cbCollapse->setChecked(false);
    lCollapse->addWidget(cbCollapse);
    connect(cbCollapse,SIGNAL(toggled(bool)),this,SLOT(setExpanded(bool)));
    connect(cbCollapse,SIGNAL(toggled(bool)),editor,SLOT(setExpanded(bool)));
    // Monitor
    _fPreview = new QFrame(this);
    lRightArea->addWidget(_fPreview);
    QVBoxLayout *lPreview = new QVBoxLayout(_fPreview);
    _fPreview->setObjectName("view");
    _fPreview->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    _fPreview->setMinimumSize(200, 150);
    UiProgramView *pView = new UiProgramView(program, _fPreview);
    QSizePolicy policy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    lPreview->addWidget(pView);
    lRightArea->addStretch();

    setExpanded(program->isOnAir());
    editor->setExpanded(program->isOnAir());

    connect(_program, SIGNAL(destroyed()), this, SLOT(deleteLater()));
    connect(_program, SIGNAL(onAir(bool)), bOnAir, SLOT(setChecked(bool)));
    connect(_program, SIGNAL(onAir(bool)), this, SLOT(updateOnAirStatus(bool)));

    bOnAir->blockSignals(true);
    bOnAir->setChecked(_program->isOnAir());
    bOnAir->blockSignals(false);

    cbCollapse->blockSignals(true);
    cbCollapse->setChecked(_program->isOnAir());
    cbCollapse->blockSignals(false);
    updateOnAirStatus(_program->isOnAir());
}

void UiProgram::setExpanded(bool expanded)
{
    _fPreview->setVisible(expanded);
    this->setProperty("expanded", expanded);
    _wBackground->style()->unpolish(_wBackground);
    _wBackground->style()->polish(_wBackground);

    if (expanded)
    {        
        this->setMinimumHeight(325);
        this->setMaximumHeight(325);
    }
    else
    {
        this->setMinimumHeight(145);
        this->setMaximumHeight(145);
    }

    QScrollArea * sa = dynamic_cast<QScrollArea*>(parentWidget()->parentWidget()->parentWidget());
    if(sa)
    {
        sa->ensureWidgetVisible(this,0,0);
    }
}

void UiProgram::requestMasterProgramChange(bool on)
{
    if(on)
        _program->minotor()->master()->setProgram(_program);
}

void UiProgram::updateOnAirStatus(bool onAir)
{
    this->setProperty("onair", onAir);
    _wBorder->style()->unpolish(_wBorder);
    _wBorder->style()->polish(_wBorder);
}

void UiProgram::exportProgram()
{
    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), dataPath,tr(" (*.mpr)"));
    if(QFile::exists(fileName))
    {
        QFile::remove(fileName);
    }
    QSettings parser(fileName, QSettings::IniFormat);
    _program->minotor()->save(_program, &parser);
}

void UiProgram::setHighlight(bool on)
{
    _highlight = on;

    this->setProperty("highlight", on);
    _wHighlight->style()->unpolish(_wHighlight);
    _wHighlight->style()->polish(_wHighlight);
}
