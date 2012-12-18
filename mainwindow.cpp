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

unsigned char currentRedValue;
unsigned char currentGreenValue;
unsigned char currentBlueValue;

bool isPlaying = false;

unsigned int nbClock=0;

void MainWindow::handleClock(void)
{
    if(isPlaying)
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

        const int currentTime = (qreal(animation.duration())) * (((qreal)nbClock) / 24.0);
        animation.setCurrentTime(currentTime);
        static int currentAngle = 0;
        const int delta = animation.currentValue().toInt() - currentAngle;
        ui->graphicsView->rotate(delta);
        currentAngle += delta;
        _ledMatrix->showView(ui->graphicsView);

        /*
        // if ((nbClock%24)==0) {
             ui->graphicsView->rotate(1);
             _ledMatrix->showView(ui->graphicsView);
        // }
        */

        // Clock counter
        if (nbClock==23)
        {
            nbClock = 0;
        }
        else
        {
            nbClock++;
        }
    }
}

void MainWindow::handleStop(void)
{
    isPlaying = false;
}

void MainWindow::handleStart(void)
{
    nbClock = 0;
    isPlaying = true;
}

void MainWindow::handleContinue(void)
{
    isPlaying = true;
}

#define MIDI_CLOCK      248
#define MIDI_STOP       252
#define MIDI_START      250
#define MIDI_CONTINUE   251

void MainWindow::midiCallback(double deltatime, std::vector< unsigned char > *message)
{
    unsigned int nBytes = message->size();

    const unsigned char command = message->at(0);
    switch(command) {
    case MIDI_CLOCK: handleClock(); break;
    case MIDI_STOP: handleStop(); break;
    case MIDI_START: handleStart(); break;
    case MIDI_CONTINUE: handleContinue(); break;
    default:
        qDebug() << "no handler for" << command;
    }

    /*
    for ( unsigned int i=0; i<nBytes; i++ )
    {
        qDebug() << "Byte " << i << " = " << (int)message->at(i) << ", ";
        if ( nBytes > 0 )
          qDebug() << "stamp = " << deltatime;
    }
    */

}

void _midiCallback(double deltatime, std::vector< unsigned char > *message, void *userData )
{
    ((MainWindow*)userData)->midiCallback(deltatime, message);
}

void MainWindow::midiConnect(unsigned int portIndex)
{
    _midiIn->openPort(portIndex);

    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue.
    _midiIn->setCallback( &_midiCallback, this );

    // Don't ignore sysex, timing, or active sensing messages.
    _midiIn->ignoreTypes( false, false, false );


}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

    animation.setStartValue(QVariant(0));
    animation.setEndValue(QVariant(360));

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
        ui->pbConnectSerial->setDisabled(true);
    }

    //Midi management
    _midiIn = new RtMidiIn();

    // Check available ports.
    unsigned int nPorts = _midiIn->getPortCount();
    for (int i=0;i<nPorts;i++)
    {
        ui->cbMidiPort->addItem(_midiIn->getPortName(i).c_str());
    }
    if ( nPorts == 0 ) {
        ui->pbConnectMidi->setDisabled(true);
        delete _midiIn;
    }

    //qDebug(port->errorString());
    ui->textEditRed->setText("0");
    ui->textEditGreen->setText("0");
    ui->textEditBlue->setText("0");
}

MainWindow::~MainWindow()
{
    delete _ledMatrix;
    delete _midiIn;
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
    midiConnect(ui->cbMidiPort->currentIndex());
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
