#include "configdialog.h"
#include "ui_configdialog.h"

#include "qextserialenumerator.h"

#include <QDebug>
#include <QMetaProperty>
#include <QSettings>

#include "minotor.h"
#include "uimidiinterface.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    dynamic_cast<QVBoxLayout*>(ui->wMidiInterfaces->layout())->addStretch(1);

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
    _settings.beginGroup("midi");
    Midi *midi = Minotor::minotor()->midi();

    _settings.beginGroup("interface");
    foreach(QString group, _settings.childGroups())
    {
        // Use id as group in settings (ie: midi/interface/0)
        _settings.beginGroup(group);

        if(!_settings.contains("name"))
        {
            qDebug() << Q_FUNC_INFO
                     << "Error while loading: midi interface name";
            break;
        }
        const QString midiPort = _settings.value("name").toString();
        MidiInterface *midiInterface = midi->interface(midiPort);
        if(!midiInterface)
        {
            // Midi interface is not detected, lets create it!
            midiInterface = midi->addMidiInterface(midiPort);
        }
        if(midiInterface)
        {
            midiInterface->setId(group.toInt());

            connect(midiInterface, SIGNAL(connected(bool)), ui->cbMidiPort_1, SLOT(setDisabled(bool)));
            connect(midiInterface, SIGNAL(connected(bool)), ui->pbMidiConnect_1, SLOT(setChecked(bool)));

            QObject *object = static_cast<QObject*>(midiInterface);
            for(int j=0; j<object->metaObject()->propertyCount(); j++)
            {
                QMetaProperty omp = object->metaObject()->property(j);
                // "name" key has already been used
                if((omp.name() != QString("name")) && (omp.name() != QString("objectName")))
                {
                    bool ok = omp.write(object, _settings.value(omp.name()));
                    qDebug() << Q_FUNC_INFO
                             << QString("object property: %1 loaded: %2").arg(omp.name()).arg(QVariant(ok).toString());
                }
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO
                     << "No midi interface found with portName:" << midiPort;
        }
        _settings.endGroup(); // group id
    }
    _settings.endGroup(); // "interface" group
    _settings.endGroup(); // "midi" group

    // Hack to refresh list at startup
    this->on_tabWidget_currentChanged(0);
    this->on_tabWidget_currentChanged(1);
    this->on_tabWidget_currentChanged(2);
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
    disconnect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));

    switch(index)
    {
    case 0: //Midi
    {
        Midi *midi = Minotor::minotor()->midi();
        foreach (UiMidiInterface *uiInterface, ui->wMidiInterfaces->findChildren<UiMidiInterface*>())
        {
            delete(uiInterface);
        }
        foreach (QFrame *separator, ui->wMidiInterfaces->findChildren<QFrame*>())
        {
            delete(separator);
        }
        foreach (MidiInterface *interface, midi->interfaces())
        {
            UiMidiInterface *uiInterface = new UiMidiInterface(interface,this);
            dynamic_cast<QVBoxLayout*>(ui->wMidiInterfaces->layout())->insertWidget(ui->wMidiInterfaces->layout()->count()-1,uiInterface);
            QFrame *fSeparator = new QFrame(ui->wMidiInterfaces);
            fSeparator->setObjectName("interfaceline");
            fSeparator->setFrameShape(QFrame::HLine);
            fSeparator->setFrameShadow(QFrame::Sunken);
            fSeparator->setLineWidth(1);
            dynamic_cast<QVBoxLayout*>(ui->wMidiInterfaces->layout())->insertWidget(ui->wMidiInterfaces->layout()->count()-1,fSeparator);
        }
    }
    case 1: // MIDI mapping
    {
        int currentItem = -1;
        const QStringList midiAvailablePorts = Minotor::minotor()->midi()->getPorts();
        const QString midiPort = ui->cbMidiPort_1->itemText(ui->cbMidiPort_1->currentIndex());
        if(midiAvailablePorts.contains(midiPort))
        {
            currentItem = midiAvailablePorts.indexOf(midiPort);
        }
        ui->cbMidiPort_1->clear();
        ui->cbMidiPort_1->addItems(midiAvailablePorts);
        ui->cbMidiPort_1->setCurrentIndex(currentItem);

        // MIDI Mapping
        connect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
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
    case 2: // Serial tab
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

        _settings.beginGroup("midi");
        _settings.beginGroup("interface");
        // Remove all interfaces
        _settings.remove("");
        MidiInterfaces midiInterfaces = Minotor::minotor()->midi()->interfaces();
        int id = 0;
        for(int i=0; i<midiInterfaces.count(); i++)
        {
            MidiInterface *midiInterface = midiInterfaces.at(i);
            if(midiInterface->isUsed())
            {
                _settings.beginGroup(QString::number(id));
                QObject *object = static_cast<QObject*>(midiInterface);
                for(int j=0; j<object->metaObject()->propertyCount(); j++)
                {
                    QMetaProperty omp = object->metaObject()->property(j);
                    // Dont store "objectName"
                    if(omp.name() != QString("objectName"))
                    {
                        _settings.setValue(omp.name(), omp.read(object));
                    }
                }
                _settings.endGroup();
                id++;
            }
        }
        _settings.endGroup(); // interface
        _settings.endGroup(); // midi
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

void ConfigDialog::configDialogFinished(int result)
{
    qDebug() << "ConfigDialog: result" << result;
    disconnect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
}

void ConfigDialog::on_pbMidiConnect_1_clicked(bool checked)
{
    QString portName = ui->cbMidiPort_1->currentText();
    MidiInterface *midiInterface = Minotor::minotor()->midi()->interface(portName);
    if(midiInterface)
    {
        if (checked)
        {
            if(!midiInterface->open())
            {
                qDebug() << Q_FUNC_INFO
                         << "Unable to open MIDI port:" << portName;
            }
        } else {
            midiInterface->close();
        }
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "Could not find midi interface:" << portName;
    }
}
