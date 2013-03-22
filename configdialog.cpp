#include "configdialog.h"
#include "ui_configdialog.h"

#include "qextserialenumerator.h"

#include <QtCore/QDebug>
#include <QSettings>

#include "minotor.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    // Hack to refresh list at startup
    this->on_tabWidget_currentChanged(1);
    this->on_tabWidget_currentChanged(0);

    QSettings _settings(QSettings::IniFormat, QSettings::UserScope, QString("Minotor"));

    connect(Minotor::minotor()->ledMatrix(), SIGNAL(connected(bool)), ui->pbSerialConnect, SLOT(setChecked(bool)));
    connect(Minotor::minotor()->ledMatrix(), SIGNAL(connected(bool)), ui->cbSerialPort, SLOT(setDisabled(bool)));

    connect(this, SIGNAL(finished(int)), this, SLOT(configDialogFinished(int)));
    // TODO: Search in port list
    Minotor::minotor()->ledMatrix()->openPortByName(_settings.value("serial/interface").toString());
    if (Minotor::minotor()->ledMatrix()->isConnected())
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
        if(portnames.contains(Minotor::minotor()->ledMatrix()->portName()))
        {
            currentItem = portnames.indexOf(Minotor::minotor()->ledMatrix()->portName());
        }
        ui->cbSerialPort->setCurrentIndex(currentItem);
    }

    // MIDI
    connect(Minotor::minotor()->midi(), SIGNAL(connected(bool)), ui->cbMidiPort, SLOT(setDisabled(bool)));
    connect(Minotor::minotor()->midi(), SIGNAL(connected(bool)), ui->pbMidiConnect, SLOT(setChecked(bool)));

    const QString midiPort = _settings.value("midi/interface").toString();
    if (Minotor::minotor()->midi()->getPorts().contains(midiPort))
    {
        if(Minotor::minotor()->midi()->openPort(Minotor::minotor()->midi()->getPorts().indexOf(midiPort)))
        {
            ui->cbMidiPort->setCurrentIndex(Minotor::minotor()->midi()->getPorts().indexOf(midiPort));
        }
    }
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::midiControlChanged(const int interface, const quint8 channel, const quint8 control, const quint8 value)
{
    bool found = false;
    int row;
    for(row = 0; row < ui->tableMidiMapping->rowCount(); row++)
    {
        if(interface == ui->tableMidiMapping->item(row, 0)->text().toInt() &&
                channel == ui->tableMidiMapping->item(row, 1)->text().toInt() &&
                control == ui->tableMidiMapping->item(row, 2)->text().toInt())
        {
            ui->tableMidiMapping->item(row, 3)->setText(QString::number(value));
            found = true;
            break;
        }
    }
    if(!found)
    {
        addMidiControl(row, interface, channel, control, value);

        row++;
    }
}

void ConfigDialog::addMidiControl(const int row, const int interface, const quint8 channel, const quint8 control, const quint8 value)
{
    ui->tableMidiMapping->insertRow(row);
    QTableWidgetItem *item;
    // Interface
    item = new QTableWidgetItem(QString::number(interface));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableMidiMapping->setItem(row, 0, item);
    // Channel
    item = new QTableWidgetItem(QString::number(channel));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableMidiMapping->setItem(row, 1, item);
    // Control
    item = new QTableWidgetItem(QString::number(control));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableMidiMapping->setItem(row, 2, item);
    // Value
    item = new QTableWidgetItem(QString::number(value));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableMidiMapping->setItem(row, 3, item);
    ui->lMidiControlsCount->setText(QString::number(ui->tableMidiMapping->rowCount()));
}

void ConfigDialog::on_tabWidget_currentChanged(int index)
{
    disconnect(Minotor::minotor(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));

    switch(index)
    {
    case 0: // MIDI tab
    {
        int currentItem = -1;
        if(Minotor::minotor()->midi()->getPorts().contains(ui->cbMidiPort->itemText(ui->cbMidiPort->currentIndex())))
        {
            currentItem = Minotor::minotor()->midi()->getPorts().indexOf(ui->cbMidiPort->itemText(ui->cbMidiPort->currentIndex()));
        }
        ui->cbMidiPort->clear();
        ui->cbMidiPort->addItems(Minotor::minotor()->midi()->getPorts());
        ui->cbMidiPort->setCurrentIndex(currentItem);

        connect(Minotor::minotor(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));

        const MidiControlList midiControls = Minotor::minotor()->midiMapping()->midiControls();
        ui->tableMidiMapping->clear();

        ui->tableMidiMapping->setColumnCount(4);
        //Set Header Label Texts
        ui->tableMidiMapping->setHorizontalHeaderLabels(QString("Interface;Channel;Control;Value").split(";"));

        int row = 0;
        foreach(MidiControl *midiControl, midiControls)
        {
            addMidiControl(row, midiControl->interface(), midiControl->channel(), midiControl->control(), midiControl->value());
            row++;
        }

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
        QSettings _settings(QSettings::IniFormat, QSettings::UserScope, QString("Minotor"));
        _settings.setValue("serial/interface", Minotor::minotor()->ledMatrix()->portName());
        _settings.setValue("midi/interface", Minotor::minotor()->midi()->portName());
        _settings.sync();
    }
}

void ConfigDialog::on_pbSerialConnect_clicked(bool checked)
{
    if (checked)
    {
        Minotor::minotor()->ledMatrix()->openPortByName(ui->cbSerialPort->itemText(ui->cbSerialPort->currentIndex()));
        if(!Minotor::minotor()->ledMatrix()->isConnected())
        {
            qDebug() << "Unable to connect to serial port";
        }
    } else {
        Minotor::minotor()->ledMatrix()->closePort();
    }
}

void ConfigDialog::on_pbMidiConnect_clicked(bool checked)
{
    if (checked)
    {
        if(!Minotor::minotor()->midi()->openPort(ui->cbMidiPort->currentIndex()))
        {
            qDebug() << "Unable to open MIDI port";
        }
    } else {
        Minotor::minotor()->midi()->closePort();
    }
}

void ConfigDialog::configDialogFinished(int result)
{
    qDebug() << "ConfigDialog: result" << result;
    disconnect(Minotor::minotor(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
}
