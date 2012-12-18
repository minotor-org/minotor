#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qextserialport.h"
#include <sstream>
#include "qextserialenumerator.h"
#include <QtCore/QList>
#include <QtCore/QDebug>
#include "RtMidi.h"

QextSerialPort * port;
RtMidiIn * midiIn;
#define NB_PIXELS 384

char frame[NB_PIXELS*3];
char currentRedValue;
char currentGreenValue;
char currentBlueValue;

bool isPlaying = false;

unsigned int nbClock=0;

void handleClock(void)
{
    if(isPlaying)
    {
        if ((nbClock%12)==0)
        {
            /*
        for ( unsigned int i=0; i<nBytes; i++ )
        {
            qDebug() << "Byte " << i << " = " << (int)message->at(i) << ", ";
            if ( nBytes > 0 )
                qDebug() << "stamp = " << deltatime;
        }
        */

            //LEDS are BRG
            for (int i=0;i<NB_PIXELS;i++)
            {
                frame[(i*3)+0] = 0xff;
                frame[(i*3)+1] = 0xff;
                frame[(i*3)+2] = 0xff;

            }
            port->write(frame,(NB_PIXELS*3));
            char endFrame = 0x01;
            port->write(&endFrame,1);
            //qDebug("frame flash sent");
        }
        else if ((nbClock%12) ==1)
        {
            for (int i=0;i<NB_PIXELS;i++)
            {
                frame[(i*3)+0] = 0x00;
                frame[(i*3)+1] = 0x00;
                frame[(i*3)+2] = 0x00;
            }
            port->write(frame,(NB_PIXELS*3));
            char endFrame = 0x01;
            port->write(&endFrame,1);
        }

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

void handleStop(void)
{
    isPlaying = false;
}

void handleStart(void)
{
    nbClock = 0;
    isPlaying = true;
}

void handleContinue(void)
{
    isPlaying = true;
}

#define MIDI_CLOCK      248
#define MIDI_STOP       252
#define MIDI_START      250
#define MIDI_CONTINUE   251

void midiCallback( double deltatime, std::vector< unsigned char > *message, void *userData )
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

void midiConnect(unsigned int portIndex)
{
    midiIn->openPort(portIndex);

    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue.
    midiIn->setCallback( &midiCallback );

    // Don't ignore sysex, timing, or active sensing messages.
    midiIn->ignoreTypes( false, false, false );


}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
        port = new QextSerialPort(ui->cbSerialPort->itemText(0));
        port->setBaudRate(BAUD1000000);
        port->open(QIODevice::WriteOnly);
        ui->pbConnectSerial->setDisabled(true);
    }

    //Midi management
    midiIn = new RtMidiIn();

    // Check available ports.
    unsigned int nPorts = midiIn->getPortCount();
    for (int i=0;i<nPorts;i++)
    {
        ui->cbMidiPort->addItem(midiIn->getPortName(i).c_str());
    }
    if ( nPorts == 0 ) {
        ui->pbConnectMidi->setDisabled(true);
        delete midiIn;
    }

    //qDebug(port->errorString());
    ui->textEditRed->setText("0");
    ui->textEditGreen->setText("0");
    ui->textEditBlue->setText("0");
    //for (int i=0;i<NB_PIXELS;i++)
    //{
    //    fraport->open(QIODevice::WriteOnly);me[(i*3)+0] = 0xff;
    //    frame[(i*3)+1] = 0x00;
    //    frame[(i*3)+2] = 0xff;
    //}
    //port->write(frame,(NB_PIXELS*3));
}

MainWindow::~MainWindow()
{
    delete midiIn;
    port->close();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //LEDS are BRG
    for (int i=0;i<NB_PIXELS;i++)
    {
        frame[(i*3)+0] = (currentBlueValue==0x01)?0:currentBlueValue;
        frame[(i*3)+1] = (currentRedValue==0x01)?0:currentRedValue;
        frame[(i*3)+2] = (currentGreenValue==0x01)?0:currentGreenValue;
    }

    port->write(frame,(NB_PIXELS*3));
    char endFrame = 0x01;
    port->write(&endFrame,1);
    //qDebug(port->errorString());
    qDebug("frame sent");
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


void MainWindow::on_pushButton_2_clicked()
{
    port->close();
}

void MainWindow::on_pbConnectMidi_clicked()
{
    midiConnect(ui->cbMidiPort->currentIndex());
}
