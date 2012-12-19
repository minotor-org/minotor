#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>
#include "qextserialenumerator.h"
#include <QtCore/QList>
#include <QtCore/QDebug>

#include <QGraphicsView>
#include <QColor>
#include <QRgb>
#include <QPainter>
#include <QDateTime>

unsigned char currentRedValue;
unsigned char currentGreenValue;
unsigned char currentBlueValue;


void MainWindow::handleClock(void)
{
    if(_isSequenceRunning)
    {
        /*
        // Flash
        if ((nbClock%24)==0)
        {
           _ledMatrix->fill(Qt::white);
           _ledMatrix->show();
        }
        else if ((nbClock%24) ==1)
        {
            _ledMatrix->fill(Qt::black);
            _ledMatrix->show();
        }
        */

        static qreal currentScale = 1.0;

        const int currentTime = (qreal(animation.duration())) * (((qreal)_ppqnId) / 24.0);
        animation.setCurrentTime(currentTime);
        const qreal delta = animation.currentValue().toReal()/currentScale;
        ui->graphicsView->scale(delta,delta);
        currentScale *= delta;
        qDebug() << currentScale << "delta :" << delta;
        _ledMatrix->showView(ui->graphicsView);

/*
        const int currentTime = (qreal(animation.duration())) * (((qreal)nbClock) / 24.0);
        animation.setCurrentTime(currentTime);
        static int currentAngle = 0;
        const int delta = animation.currentValue().toInt() - currentAngle;
        ui->graphicsView->rotate(delta);
        currentAngle += delta;
        _ledMatrix->showView(ui->graphicsView);
*/
        /*
        // if ((nbClock%24)==0) {
             ui->graphicsView->rotate(1);
             _ledMatrix->showView(ui->graphicsView);
        // }
        */

        // Clock counter
        if (_ppqnId==23) { _ppqnId = 0; } else { _ppqnId++; }
    }
}

void MainWindow::handleStop(void)
{
    _isSequenceRunning = false;
}

void MainWindow::handleStart(void)
{
    _ppqnId = 0;
    _isSequenceRunning = true;
}

void MainWindow::handleContinue(void)
{
    _isSequenceRunning = true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _ppqnId(0),
    _isSequenceRunning(false)
{
    ui->setupUi(this);
    _scene.setSceneRect(QRectF(0, 0, 200, 200));
    _scene.addRect(QRectF(50, 50, 100, 100), QPen(Qt::red), QBrush(Qt::green));
    _scene.addRect(QRectF(80, 80, 100, 100), QPen(Qt::blue), QBrush(Qt::white));

    QPixmap background;
    background.fill(Qt::black);
    ui->graphicsView->setScene(&_scene);
    ui->graphicsView->setBackgroundBrush(background);
    ui->graphicsView->show();
    ui->graphicsViewMatrix->setScene(&_scene);
    ui->graphicsViewMatrix->show();

    animation.setStartValue(QVariant(0.1));
    animation.setEndValue(QVariant(3.0));

    //Serial Management
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    qDebug() << "List of ports:";
    //! [2]
    foreach (QextPortInfo info, ports) {
        if (info.physName.startsWith("/dev/ttyACM"))
        {
            qDebug() << "port name:"       << info.portName;
            qDebug() << "friendly name:"   << info.friendName;
            qDebug() << "physical name:"   << info.physName;
            qDebug() << "enumerator name:" << info.enumName;
            qDebug() << "vendor ID:"       << info.vendorID;
            qDebug() << "product ID:"      << info.productID;

            qDebug() << "===================================";
            ui->cbSerialPort->addItem(info.physName);
        }

    }
    qDebug()<<ui->cbSerialPort->count();

    if (ui->cbSerialPort->count()==1)
    {
        _ledMatrix = new LedMatrix(ui->cbSerialPort->itemText(0));
        ui->cbSerialPort->setEnabled(false);
        ui->pbConnectSerial->setEnabled(false);
    }
    _midi = new Midi();
    ui->cbMidiPort->addItems(_midi->getPorts());

    if ( ui->cbMidiPort->count() == 0 ) {
        ui->pbConnectMidi->setDisabled(true);
    }

    ui->textEditRed->setText("0");
    ui->textEditGreen->setText("0");
    ui->textEditBlue->setText("0");
}

MainWindow::~MainWindow()
{
    delete _ledMatrix;
    delete _midi;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QColor color (currentRedValue, currentGreenValue, currentBlueValue);
    _ledMatrix->fill(color);
    _ledMatrix->show();
}

void MainWindow::on_horizontalSliderRed_valueChanged(int value)
{

    std::ostringstream oss;
    // écrire un nombre dans le flux
    oss << value;
    // récupérer une chaîne de caractères
    QString val  = val.fromStdString(oss.str());
    currentRedValue = value;
    ui->textEditRed->setText(val);
}

void MainWindow::on_horizontalSliderGreen_valueChanged(int value)
{
    std::ostringstream oss;
    // écrire un nombre dans le flux
    oss << value;
    // récupérer une chaîne de caractères
    QString val  = val.fromStdString(oss.str());
    currentGreenValue = value;
    ui->textEditGreen->setText(val);
}

void MainWindow::on_horizontalSliderBlue_valueChanged(int value)
{
    std::ostringstream oss;
    // écrire un nombre dans le flux
    oss << value;
    // récupérer une chaîne de caractères
    QString val  = val.fromStdString(oss.str());
    currentBlueValue = value;
    ui->textEditBlue->setText(val);
}

void MainWindow::on_pbConnectMidi_clicked()
{
    _midi->openPort(ui->cbMidiPort->currentIndex());
    connect(_midi,SIGNAL(clockReceived()),this,SLOT(handleClock()));
    connect(_midi,SIGNAL(startReceived()),this,SLOT(handleStart()));
    connect(_midi,SIGNAL(stopReceived()),this,SLOT(handleStop()));
    connect(_midi,SIGNAL(continueReceived()),this,SLOT(handleContinue()));
    ui->cbMidiPort->setEnabled(false);
    ui->pbConnectMidi->setEnabled(false);
}

void MainWindow::on_pushButton_3_clicked()
{
    //_ledMatrix->showScene(&_scene);
    _ledMatrix->showView(ui->graphicsView);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->rotate(10);
}

void MainWindow::on_pbConnectSerial_clicked()
{
    ui->cbSerialPort->setEnabled(false);
    ui->pbConnectSerial->setEnabled(false);
}
