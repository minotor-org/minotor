#include "configdialog.h"
#include "ui_configdialog.h"

#include "qextserialenumerator.h"

#include <QDebug>
#include <QMetaProperty>

#include "minotor.h"
#include "uimidiinterface.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    dynamic_cast<QVBoxLayout*>(ui->wMidiInterfaces->layout())->addStretch(1);
    connect(this, SIGNAL(finished(int)), this, SLOT(configDialogFinished(int)));

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QString("Minotor"));

    setupLedMatrix(settings);
    setupMidi(settings);

    // Hack to refresh list at startup
    this->on_tabWidget_currentChanged(0);
    updateMidiMappingTab();
    this->on_tabWidget_currentChanged(2);
}

void ConfigDialog::setupLedMatrix(QSettings &settings)
{
    LedMatrix *matrix = Minotor::minotor()->ledMatrix();
    connect(matrix, SIGNAL(connected(bool)), ui->pbSerialConnect, SLOT(setChecked(bool)));
    connect(matrix, SIGNAL(connected(bool)), ui->cbSerialPort, SLOT(setDisabled(bool)));

    // TODO: Search in port list
    matrix->openPortByName(settings.value("serial/interface").toString());
    if (matrix->isConnected())
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
        if(portnames.contains(matrix->portName()))
        {
            currentItem = portnames.indexOf(matrix->portName());
        }
        ui->cbSerialPort->setCurrentIndex(currentItem);
    }
}

void ConfigDialog::setupMidi(QSettings &settings)
{
    // MIDI
    settings.beginGroup("midi");
    Midi *midi = Minotor::minotor()->midi();

    settings.beginGroup("interface");
    foreach(QString group, settings.childGroups())
    {
        // Use id as group in settings (ie: midi/interface/0)
        settings.beginGroup(group);

        if(!settings.contains("name"))
        {
            qDebug() << Q_FUNC_INFO
                     << "Error while loading: midi interface name";
            break;
        }
        const QString midiPort = settings.value("name").toString();
        MidiInterface *midiInterface = midi->interface(midiPort);
        if(!midiInterface)
        {
            // Midi interface is not detected, lets create it!
            midiInterface = midi->addMidiInterface(midiPort);
        }
        if(midiInterface)
        {
            midiInterface->setId(group.toInt());

            QObject *object = static_cast<QObject*>(midiInterface);
            for(int j=0; j<object->metaObject()->propertyCount(); j++)
            {
                QMetaProperty omp = object->metaObject()->property(j);
                // "name" key has already been used
                if((omp.name() != QString("name")) && (omp.name() != QString("objectName")))
                {
                    bool ok = omp.write(object, settings.value(omp.name()));
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
        settings.endGroup(); // group id
    }
    settings.endGroup(); // "interface" group
    settings.endGroup(); // "midi" group
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
        Minotor::minotor()->midiMapping()->addMidiControl(interface, channel, control);
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
        updateMidiTab();
        break;
    case 1: // MIDI mapping
        updateMidiMappingTab();
        break;
    case 2: // Serial
        updateSerialTab();
        break;
    }
}

void ConfigDialog::updateMidiTab()
{
    qDebug() << Q_FUNC_INFO;
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

void ConfigDialog::updateMidiMappingTab()
{
    qDebug() << Q_FUNC_INFO;
    connect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)),Qt::UniqueConnection);
    const MidiControlList midiControls = Minotor::minotor()->midiMapping()->midiControls();
    ui->tableMidiMapping->clear();
    ui->tableMidiMapping->setColumnCount(4);
    ui->tableMidiMapping->setRowCount(0);
    //Set Header Label Texts
    ui->tableMidiMapping->setHorizontalHeaderLabels(QString("Interface;Channel;Control;Value").split(";"));

    int row = 0;
    foreach(MidiControl *midiControl, midiControls)
    {
        addMidiControl(row, midiControl->interface(), midiControl->channel(), midiControl->control(), midiControl->value());
        row++;
    }
}

void ConfigDialog::updateSerialTab()
{
    qDebug() << Q_FUNC_INFO;
    // Port enumeration
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
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

    int currentItem = -1;
    if(portnames.contains(ui->cbSerialPort->itemText(ui->cbSerialPort->currentIndex())))
    {
        currentItem = portnames.indexOf(ui->cbSerialPort->itemText(ui->cbSerialPort->currentIndex()));
    }

    // Clear combobox
    ui->cbSerialPort->clear();
    ui->cbSerialPort->addItems(portnames);
    ui->cbSerialPort->setCurrentIndex(currentItem);
}

void ConfigDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Save)
    {
        QSettings settings(QSettings::IniFormat, QSettings::UserScope, QString("Minotor"));
        settings.setValue("serial/interface", Minotor::minotor()->ledMatrix()->portName());

        settings.beginGroup("midi");
        settings.beginGroup("interface");
        // Remove all interfaces
        settings.remove("");
        MidiInterfaces midiInterfaces = Minotor::minotor()->midi()->interfaces();
        int id = 0;
        for(int i=0; i<midiInterfaces.count(); i++)
        {
            MidiInterface *midiInterface = midiInterfaces.at(i);
            if(midiInterface->isUsed())
            {
                settings.beginGroup(QString::number(id));
                QObject *object = static_cast<QObject*>(midiInterface);
                for(int j=0; j<object->metaObject()->propertyCount(); j++)
                {
                    QMetaProperty omp = object->metaObject()->property(j);
                    // Dont store "objectName"
                    if(omp.name() != QString("objectName"))
                    {
                        settings.setValue(omp.name(), omp.read(object));
                    }
                }
                settings.endGroup();
                id++;
            }
        }
        settings.endGroup(); // interface
        settings.endGroup(); // midi
        settings.sync();
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
