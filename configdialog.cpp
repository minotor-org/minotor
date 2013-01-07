#include "configdialog.h"
#include "ui_configdialog.h"

#include "qextserialenumerator.h"

#include <QtCore/QDebug>
#include <QSettings>

ConfigDialog::ConfigDialog(LedMatrix *ledMatrix, Midi *midi, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog),
    _ledMatrix(ledMatrix),
    _midi(midi)
{
    ui->setupUi(this);

    // Hack to refresh list at startup
    this->on_tabWidget_currentChanged(1);
    this->on_tabWidget_currentChanged(0);

    // TODO: Remove hardcoded filename
    QSettings _settings("/home/gotleg/minotor.ini",
                       QSettings::IniFormat);

    connect(_ledMatrix, SIGNAL(connected(bool)), ui->pbSerialConnect, SLOT(setChecked(bool)));
    connect(_ledMatrix, SIGNAL(connected(bool)), ui->cbSerialPort, SLOT(setDisabled(bool)));

    // TODO: Search in port list
    _ledMatrix->openPortByName(_settings.value("serial/interface").toString());
    if (_ledMatrix->isConnected())
    {
        // Port enumeration
        QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
        QStringList portnames;

        foreach (QextPortInfo info, ports) {
            if (1 || info.physName.startsWith("/dev/ttyACM"))
            {
                portnames.append(info.physName);
            }
        }

        int currentItem = -1;
        if(portnames.contains(_ledMatrix->portName()))
        {
            currentItem = portnames.indexOf(_ledMatrix->portName());
        }
        ui->cbSerialPort->setCurrentIndex(currentItem);
    }

    // MIDI
    connect(_midi, SIGNAL(connected(bool)), ui->cbMidiPort, SLOT(setDisabled(bool)));
    connect(_midi, SIGNAL(connected(bool)), ui->pbMidiConnect, SLOT(setChecked(bool)));

    const QString midiPort = _settings.value("midi/interface").toString();
    if (_midi->getPorts().contains(midiPort))
    {
        if(_midi->openPort(_midi->getPorts().indexOf(midiPort)))
        {
            ui->cbMidiPort->setCurrentIndex(_midi->getPorts().indexOf(midiPort));
        }
    }
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::on_tabWidget_currentChanged(int index)
{
    switch(index)
    {
    case 0: // MIDI tab
    {
        int currentItem = -1;
        if(_midi->getPorts().contains(ui->cbMidiPort->itemText(ui->cbMidiPort->currentIndex())))
        {
            currentItem = _midi->getPorts().indexOf(ui->cbMidiPort->itemText(ui->cbMidiPort->currentIndex()));
        }
        ui->cbMidiPort->clear();
        ui->cbMidiPort->addItems(_midi->getPorts());
        ui->cbMidiPort->setCurrentIndex(currentItem);
    }
        break;
    case 1: // Serial tab
        // Port enumeration
        QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
//        qDebug() << "List of ports:";
        QStringList portnames;

        foreach (QextPortInfo info, ports) {
            if (1 || info.physName.startsWith("/dev/ttyACM"))
            {
//                qDebug() << "port name:"       << info.portName;
//                qDebug() << "friendly name:"   << info.friendName;
//                qDebug() << "physical name:"   << info.physName;
//                qDebug() << "enumerator name:" << info.enumName;
//                qDebug() << "vendor ID:"       << info.vendorID;
//                qDebug() << "product ID:"      << info.productID;

//                qDebug() << "===================================";
                portnames.append(info.physName);
            }
        }
//        qDebug() << portnames;

        int currentItem = -1;
//        qDebug() << ui->cbSerialPort->currentIndex() << ui->cbSerialPort->itemText(ui->cbSerialPort->currentIndex());
        if(portnames.contains(ui->cbSerialPort->itemText(ui->cbSerialPort->currentIndex())))
        {
            currentItem = portnames.indexOf(ui->cbSerialPort->itemText(ui->cbSerialPort->currentIndex()));
        }

        // Clear combobox
        ui->cbSerialPort->clear();
        ui->cbSerialPort->addItems(portnames);
//        qDebug() << currentItem;
        ui->cbSerialPort->setCurrentIndex(currentItem);
        break;
    }
}

void ConfigDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Save)
    {
        QSettings _settings("/home/gotleg/minotor.ini",
                           QSettings::IniFormat);
        _settings.setValue("serial/interface", _ledMatrix->portName());
        _settings.setValue("midi/interface", _midi->portName());
        _settings.sync();
    }
}

void ConfigDialog::on_pbSerialConnect_clicked(bool checked)
{
    if (checked)
    {
        _ledMatrix->openPortByName(ui->cbSerialPort->itemText(ui->cbSerialPort->currentIndex()));
        if(!_ledMatrix->isConnected())
        {
            qDebug() << "Unable to connect to serial port";
        }
    } else {
        _ledMatrix->closePort();
    }
}

void ConfigDialog::on_pbMidiConnect_clicked(bool checked)
{
    if (checked)
    {
        if(!_midi->openPort(ui->cbMidiPort->currentIndex()))
        {
            qDebug() << "Unable to open MIDI port";
        }
    } else {
        _midi->closePort();
    }
}
