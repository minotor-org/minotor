#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include <QtCore/QList>
#include <QtCore/QDebug>
#include <QSettings>

#include "uimonitor.h"
#include "uidial.h"

#include "minoanimation.h"
#include "minoanimationproperty.h"

unsigned char currentRedValue;
unsigned char currentGreenValue;
unsigned char currentBlueValue;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _midi = new Midi();
    _minotor = new Minotor(_midi);
    ui->graphicsView->setScene(_minotor->scene());
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::black));
    ui->graphicsView->show();

    // LED Matrix
    _ledMatrix = new LedMatrix();
    _minotor->setLedMatrix(_ledMatrix);

    // Monitor
    UiMonitor *uiMonitor = new UiMonitor(ui->frame);
    uiMonitor->setGeometry(10, 10, 240, 160);
    uiMonitor->setLedMatrix(_ledMatrix);
    connect(_ledMatrix,SIGNAL(updated()),uiMonitor,SLOT(update()));

    // Configuration dialog box
    _configDialog = new ConfigDialog(_ledMatrix, _midi, this);

    ui->textEditRed->setText("0");
    ui->textEditGreen->setText("0");
    ui->textEditBlue->setText("0");

    _midiCaptureAction = new QAction("Assign MIDI control", this);
    connect(_midiCaptureAction, SIGNAL(triggered()), this, SLOT(midiCaptureTrigged()));

    foreach (MinoAnimation *animation, _minotor->animations())
    {
        foreach (MinoAnimationProperty *property, animation->properties())
        {
            UiDial *d = new UiDial(property, ui->saAnimationProperties);
            connect(d, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
        }
    }

}

MainWindow::~MainWindow()
{
    delete _configDialog;
    delete _ledMatrix;
    delete _midiCaptureAction;
    delete _midi;
    delete ui;
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
    QMenu menu(this);
    _midiCaptureAction->setParent(QObject::sender());
    menu.addAction(_midiCaptureAction);
    menu.exec(QCursor::pos());
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

/*
void MainWindow::on_pushButton_3_clicked()
{
    //_ledMatrix->showScene(&_scene);
    _ledMatrix->showView(ui->graphicsView);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->rotate(10);
}
*/

void MainWindow::on_action_Configuration_triggered()
{
    _configDialog->show();
}
