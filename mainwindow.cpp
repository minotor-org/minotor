#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include <QtCore/QList>
#include <QtCore/QDebug>
#include <QSettings>

#include <QGraphicsProxyWidget>
#include <QGraphicsView>

#include "uimastermonitor.h"
#include "uidial.h"
#include "uianimation.h"

#include "minoanimation.h"
#include "minoanimationproperty.h"
#include "minochannel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _minotor = new Minotor(this);

    ui->gvChannel1->setScene(_minotor->channel1()->scene());
    ui->gvChannel1->setBackgroundBrush(QBrush(Qt::black));
    ui->gvChannel1->show();

    ui->gvChannel2->setScene(_minotor->channel2()->scene());
    ui->gvChannel2->setBackgroundBrush(QBrush(Qt::black));
    ui->gvChannel2->show();

    _actionMidiCapture = new QAction("Assign MIDI control", this);
    connect(_actionMidiCapture, SIGNAL(triggered()), this, SLOT(midiCaptureTrigged()));

    // Configuration dialog box
    _configDialog = new ConfigDialog(_minotor, this);

    //add master
    _uiMaster = new UiMaster(_minotor, ui->fMaster);
    new QHBoxLayout(ui->fMaster);
    ui->fMaster->layout()->addWidget(_uiMaster);


    _uiChannel1 = new UiChannel(_minotor->channel1(), ui->fChannel1);
    new QHBoxLayout(ui->fChannel1);
    ui->fChannel1->layout()->addWidget(_uiChannel1);
    connect(_uiChannel1, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));

    _uiChannel2 = new UiChannel(_minotor->channel2(), ui->fChannel2);
    new QHBoxLayout(ui->fChannel2);
    ui->fChannel2->layout()->addWidget(_uiChannel2);
    connect(_uiChannel2, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));

    // Default MIDI menu
    _menu.addAction(_actionMidiCapture);

    // Transport
    ui->lcdBpm->display(_minotor->clockSource()->bpm());
    connect(_minotor->clockSource(), SIGNAL(bpmChanged(double)), ui->lcdBpm, SLOT(display(double)));
    connect(ui->pbTransportTapping, SIGNAL(pressed()), _minotor->clockSource(), SLOT(uiTapOn()));
    connect(ui->pbTransportStart, SIGNAL(pressed()), _minotor->clockSource(), SLOT(uiStart()));
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
