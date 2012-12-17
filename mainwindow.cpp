#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qextserialport.h"
#include <sstream>

QextSerialPort * port;
#define NB_PIXELS 384

char frame[NB_PIXELS*3];
char currentRedValue;
char currentGreenValue;
char currentBlueValue;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    port = new QextSerialPort("/dev/ttyACM1");
    port->setBaudRate(BAUD1000000);
    port->open(QIODevice::WriteOnly);
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
    //port->close();
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
