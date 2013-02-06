#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include <QtCore/QList>
#include <QtCore/QDebug>

#include <QToolBar>
#include <QToolButton>
#include <QLCDNumber>
#include <QSplitter>

#include "uidial.h"
#include "uianimationdescription.h"
#include "uianimationpicker.h"
#include "uianimationproperty.h"

#include "minoanimation.h"
#include "minoproperty.h"
#include "minoprogram.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _minotor = Minotor::minotor();

    ui->gvGlobal->setScene(_minotor->scene());

    _actionMidiCapture = new QAction("Assign MIDI control", this);
    connect(_actionMidiCapture, SIGNAL(triggered()), this, SLOT(midiCaptureTrigged()));

    ui->dwDebug->hide();

    // Configuration dialog box
    _configDialog = new ConfigDialog(_minotor, this);

    QHBoxLayout *lCentralWidget = new QHBoxLayout(ui->centralWidget);
    // Programs editor (central)
    QSplitter *sProgramsEditor = new QSplitter(Qt::Vertical, ui->centralWidget);
    QVBoxLayout *lProgramsEditor = new QVBoxLayout(sProgramsEditor);
    _uiMasterEditor = new UiProgramEditor(_minotor->master()->program(), sProgramsEditor);
    lProgramsEditor->addWidget(_uiMasterEditor);
    connect(_uiMasterEditor, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));

    foreach(MinoProgram* program, _minotor->programs())
    {
        UiProgramEditor * editor = new UiProgramEditor(program, sProgramsEditor);
        connect(editor, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
        lProgramsEditor->addWidget(editor);
        lCentralWidget->addWidget(sProgramsEditor);
    }

    // Views and controls (right side)
    QFrame *fViewsAndControls = new QFrame(ui->centralWidget);
    QVBoxLayout *lViewsAndControls = new QVBoxLayout(fViewsAndControls);
    _uiMaster = new UiMaster(_minotor->master(), fViewsAndControls);
    lViewsAndControls->addWidget(_uiMaster);
    lViewsAndControls->addStretch();
    _uiCue = new UiCue(_minotor->programs().at(0), fViewsAndControls);
    lViewsAndControls->addWidget(_uiCue);
    lCentralWidget->addWidget(fViewsAndControls);

    // Default MIDI menu
    _menu.addAction(_actionMidiCapture);

    // Transport
    QToolBar *tbTransport = new QToolBar("Transport", this);
    tbTransport->addAction("Tap", _minotor->clockSource(), SLOT(uiTapOn()));
    QLCDNumber *lcdBpm = new QLCDNumber();
    lcdBpm->setFrameShape(QFrame::Panel);
    lcdBpm->setFrameShadow(QFrame::Plain);
    lcdBpm->setSegmentStyle(QLCDNumber::Flat);
    lcdBpm->setMaximumHeight(17);
    lcdBpm->display(_minotor->clockSource()->bpm());
    connect(_minotor->clockSource(), SIGNAL(bpmChanged(double)), lcdBpm, SLOT(display(double)));
    tbTransport->addWidget(lcdBpm);
    tbTransport->addAction("Start", _minotor->clockSource(), SLOT(uiStart()));
    tbTransport->addAction("Stop", _minotor->clockSource(), SLOT(uiStop()));
    tbTransport->addAction("Sync", _minotor->clockSource(), SLOT(uiSync()));
    this->addToolBar(tbTransport);

    // MIDI toolbar
    QToolBar *tbMidi = new QToolBar("MIDI", this);
    QToolButton *tbMidiLearn = new QToolButton(tbMidi);
    tbMidiLearn->setText("MIDI learn");
    tbMidiLearn->setCheckable(true);
    connect(tbMidiLearn, SIGNAL(toggled(bool)), this, SLOT(tbMidiLearnToggled(bool)));
    tbMidi->addWidget(tbMidiLearn);
    this->addToolBar(tbMidi);

    // Populate animation scrollarea
    QScrollArea *sa =  new QScrollArea(ui->dwAnimationPickerContent);
    sa->setLayout(new QHBoxLayout);
    sa->layout()->addWidget(new UiAnimationPicker(sa));
    ui->dwAnimationPickerContent->layout()->addWidget(sa);
}

MainWindow::~MainWindow()
{
    delete _actionMidiCapture;
    delete _configDialog;
    delete ui;
}

void MainWindow::on_action_Configuration_triggered()
{
    _configDialog->show();
}

void MainWindow::midiCaptureTrigged()
{
    qDebug() << "here:" << QObject::sender()->metaObject()->className();

    if (QString(QObject::sender()->metaObject()->className()) == QString("QAction"))
    {
        QAction *action = ((QAction*)QObject::sender());
        qDebug() << "there:" << action->parent()->metaObject()->className();
        if (QString(action->parent()->metaObject()->className()) == QString("UiDial"))
        {
            UiDial *dial = ((UiDial*)action->parent());
            qDebug() << "midiCapture trigged for:" << dial->property()->objectName();
            _minotor->midiMapping()->assignCapturedControlTo(dial->property());
        }
    }
}

void MainWindow::customContextMenuRequested(const QPoint &pos)
{
    (void)pos;
    _actionMidiCapture->setParent(qApp->widgetAt(QCursor::pos()));
    _menu.exec(QCursor::pos());
}

void MainWindow::on_sPpqn_valueChanged(int value)
{
    const unsigned int ppqn = value%24;
    const unsigned int qn = value/24;
    _minotor->dispatchClock(value, ppqn, qn);
}

void MainWindow::tbMidiLearnToggled(bool checked)
{
    QList<UiAnimationProperty*> animationProperties = findChildren<UiAnimationProperty*>();
    foreach(UiAnimationProperty *animationProperty, animationProperties)
    {
        animationProperty->setMidiLearnMode(checked);
        animationProperty->update();
    }
}
