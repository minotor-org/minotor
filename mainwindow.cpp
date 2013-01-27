#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include <QtCore/QList>
#include <QtCore/QDebug>

#include <QToolBar>
#include <QLCDNumber>

#include "uidial.h"

#include "minoanimation.h"
#include "minoanimationproperty.h"
#include "minochannel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _minotor = new Minotor(this);

    ui->gvGlobal->setScene(_minotor->scene());
    ui->gvGlobal->setBackgroundBrush(QBrush(Qt::black));

    _actionMidiCapture = new QAction("Assign MIDI control", this);
    connect(_actionMidiCapture, SIGNAL(triggered()), this, SLOT(midiCaptureTrigged()));

    // Configuration dialog box
    _configDialog = new ConfigDialog(_minotor, this);

    // Views and controls (right side)
    QVBoxLayout *lViewsAndControls = new QVBoxLayout(ui->fViewsAndControls);
    _uiMaster = new UiMaster(_minotor->master(), ui->fViewsAndControls);
    lViewsAndControls->addWidget(_uiMaster);
    lViewsAndControls->addStretch();
    _uiCue = new UiCue(_minotor->cue(), ui->fViewsAndControls);
    lViewsAndControls->addWidget(_uiCue);

    // Channels editor (central)
    QVBoxLayout *lChannelsEditor = new QVBoxLayout(ui->fChannelsEditor);
    _uiMasterEditor = new UiChannelEditor(_minotor->master(), ui->fChannelsEditor);
    ui->fChannelsEditor->layout()->addWidget(_uiMasterEditor);
    connect(_uiMasterEditor, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    lChannelsEditor->addWidget(_uiMasterEditor);

    _uiCueEditor = new UiChannelEditor(_minotor->cue(), ui->fChannelsEditor);
    new QHBoxLayout(ui->fChannelsEditor);
    ui->fChannelsEditor->layout()->addWidget(_uiCueEditor);
    connect(_uiCueEditor, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    lChannelsEditor->addWidget(_uiCueEditor);

    lChannelsEditor->addStretch();

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
