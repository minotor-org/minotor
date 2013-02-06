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
#include "uiprogrambank.h"

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

    QVBoxLayout *lCentralWidget = new QVBoxLayout(ui->centralWidget);
    // Programs editor (central)

    //UiMaster
    _uiMaster = new UiMaster(_minotor->master(), ui->centralWidget);
    lCentralWidget->addWidget(_uiMaster);
    UiProgramBank *uiProgramBank = new UiProgramBank(_minotor, ui->centralWidget);
    lCentralWidget->addWidget(uiProgramBank);
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
    QScrollArea *sa =  new QScrollArea();
    sa->setFrameShadow(QFrame::Plain);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setWidgetResizable(true);
    UiAnimationPicker *ap = new UiAnimationPicker(sa);

    sa->setWidget(ap);
    ui->panel->layout()->addWidget(sa);
    ui->panel->layout()->setSpacing(0);
    ui->panel->layout()->setMargin(0);
    ui->panel->layout()->setContentsMargins(0,0,0,0);
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
