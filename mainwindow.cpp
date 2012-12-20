#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>
#include "qextserialenumerator.h"
#include <QtCore/QList>
#include <QtCore/QDebug>

#include "monitor.h"

unsigned char currentRedValue;
unsigned char currentGreenValue;
unsigned char currentBlueValue;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Monitor *monitor = new Monitor(this);
    monitor->setGeometry(260, 180, 240, 160);

    ui->setupUi(this);

    _midi = new Midi();
    _minotor = new Minotor(_midi);

    ui->graphicsView->setScene(_minotor->scene());
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::black));
    ui->graphicsView->show();

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
        _minotor->setLedMatrix(_ledMatrix);
        connect(_ledMatrix,SIGNAL(updated()),monitor,SLOT(update()));
        ui->cbSerialPort->setEnabled(false);
        ui->pbConnectSerial->setEnabled(false);
        monitor->setLedMatrix(_ledMatrix);
    }

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
    _ledMatrix->frame()->fill(color);
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
