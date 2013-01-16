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
    ui(new Ui::MainWindow),
    _bpmValuesCount(0),
    _bpmValuesIndex(0)
{
    ui->setupUi(this);

    _midi = new Midi();
    _minotor = new Minotor(_midi);


    ui->gvChannel1->setScene(_minotor->channel1()->scene());
    ui->gvChannel1->setBackgroundBrush(QBrush(Qt::black));
    ui->gvChannel1->show();

    ui->gvChannel2->setScene(_minotor->channel2()->scene());
    ui->gvChannel2->setBackgroundBrush(QBrush(Qt::black));
    ui->gvChannel2->show();

    ui->gvMonitor->setScene(_minotor->master()->scene());
    ui->gvMonitor->setBackgroundBrush(QBrush(Qt::black));
    ui->gvMonitor->show();

    _actionMidiCapture = new QAction("Assign MIDI control", this);
    connect(_actionMidiCapture, SIGNAL(triggered()), this, SLOT(midiCaptureTrigged()));

    // LED Matrix
    _ledMatrix = new LedMatrix();
    _minotor->setLedMatrix(_ledMatrix);

    // Configuration dialog box
    _configDialog = new ConfigDialog(_ledMatrix, _midi, this);

    //add master
    _uiMaster = new UiMaster(_minotor,ui->fMaster);
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


    qDebug() << "master scene:" << _minotor->master()->scene()->sceneRect();

    connect(ui->sPpqn, SIGNAL(valueChanged(int)), _minotor, SLOT(animate(int)));

    // Default MIDI menu
    _menu.addAction(_actionMidiCapture);

    // BPM Tapping
    _bpmTap.start(); // Note: _bmpTap is always running, tempo values are filtered if >500BPM.
    connect(&_tInternalClockGenerator, SIGNAL(timeout()), _minotor, SLOT(handleClock()));
}

MainWindow::~MainWindow()
{
    delete _actionMidiCapture;
    delete _configDialog;
    delete _ledMatrix;
    delete _midi;
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

void MainWindow::on_pbTransportTapping_clicked()
{
    int ms = _bpmTap.restart();
    if (ms < 3000) // filter tapping where BPM is less than 20 beat per minute.
    {
        _bpmValues[_bpmValuesIndex] = ms;
        _bpmValuesCount = qMin(5, _bpmValuesCount+1);
        _bpmValuesIndex = (_bpmValuesIndex+1)%5;
        qreal averageMs =  _bpmValues[0];
        for(int i=1; i<_bpmValuesCount; i++)
        {
            averageMs += _bpmValues[i];
        }
        averageMs /= _bpmValuesCount;

        const qreal bpm = (1000.0 / averageMs) * 60.0;
        qDebug() << "Tap: ms=" << ms << "average ms=" << averageMs << "(bpm" << bpm << ")" << "index" << _bpmValuesIndex << "count" << _bpmValuesCount;
        ui->lcdBpm->display(bpm);
    } else {
        qDebug() << "Tapping reset";
        _bpmValuesCount = 0;
        _bpmValuesIndex = 0;
        ui->lcdBpm->display(0.0);
    }
}

void MainWindow::on_pbTransportStart_clicked()
{
    qreal averageMs =  _bpmValues[0];
    for(int i=1; i<_bpmValuesCount; i++)
    {
        averageMs += _bpmValues[i];
    }
    averageMs /= _bpmValuesCount;

    averageMs /= 24;

    qDebug() << "bpm error" << (qreal)((averageMs - ((int)averageMs))*24);
    _tInternalClockGenerator.setInterval(averageMs);
    _tInternalClockGenerator.start();
    _minotor->handleStart();
}
