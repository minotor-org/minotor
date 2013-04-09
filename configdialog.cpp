#include "configdialog.h"
#include "ui_configdialog.h"

#include "qextserialenumerator.h"

#include <QDebug>
#include <QMetaProperty>
#include <QDesktopServices>
#include <QFileDialog>
#include <QSettings>
#include <QDir>

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
    updateMidiTab();
    updateMidiMappingTab();
    updateSerialTab();
    disconnect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
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
                    omp.write(object, settings.value(omp.name()));
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
        addMidiControl(row, channel, control, "FIXME", value);
        row++;
    }
}

void ConfigDialog::addMidiControl(const int row, const quint8 channel, const quint8 control, const QString& role, const quint8 value)
{
    ui->tableMidiMapping->insertRow(row);
    QTableWidgetItem *item;
    // Channel
    item = new QTableWidgetItem(QString::number(channel));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableMidiMapping->setItem(row, 0, item);
    // Control
    item = new QTableWidgetItem(QString::number(control));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableMidiMapping->setItem(row, 1, item);
    // Role
    QComboBox *cb = new QComboBox();
    cb->addItem("none");
    cb->addItems(MidiMapping::registeredRoles());
    int index;
    if((index = cb->findText(role)) == -1)
    {
        qDebug() << Q_FUNC_INFO
                 << "role not found:" << role;
        // Select "none"
        cb->setCurrentIndex(0);
    }
    else
    {
        cb->setCurrentIndex(index);
    }
    ui->tableMidiMapping->setCellWidget(row, 2, cb);
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
//    qDebug() << Q_FUNC_INFO;
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

void ConfigDialog::loadMidiMappingFiles()
{
    ui->cbMidiMapping->clear();
    ui->cbMidiMapping->addItem("Generic midi");

    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QDir dataDir(dataPath);
    foreach(QFileInfo file, dataDir.entryInfoList(QDir::Files))
    {
        addMidiMappingEntry(file.absoluteFilePath());
    }
}

void ConfigDialog::addMidiMappingEntry(QFileInfo file)
{
    QSettings mapping(file.absoluteFilePath(), QSettings::IniFormat);
    if(QSettings::NoError == mapping.status())
    {
        mapping.beginGroup("general");
        QString vendor = mapping.value("vendor", "undefined").toString();
        QString product = mapping.value("product", QVariant("undefined")).toString();
        ui->cbMidiMapping->addItem(QString("%1 - %2 (%3)").arg(vendor).arg(product).arg(file.fileName()), file.absoluteFilePath());
        mapping.endGroup();
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "unable to parse file:" << file.absoluteFilePath();
    }
}

void ConfigDialog::loadMidiMappingFile(QString file)
{
    ui->tableMidiMapping->clearContents();
    ui->tableMidiMapping->setRowCount(0);
    QSettings mapping(file, QSettings::IniFormat);
    if(QSettings::NoError == mapping.status())
    {
        // General
        mapping.beginGroup("general");
        QString vendor = mapping.value("vendor", QString("undefined")).toString();
        QString product = mapping.value("product", QString("undefined")).toString();
        ui->leVendor->setText(vendor);
        ui->leProduct->setText(product);
        mapping.endGroup();

        // MIDI Controls
        int size = mapping.beginReadArray("midi_controls");
        for(int i = 0; i < size; ++i)
        {
            mapping.setArrayIndex(i);
            const int row = ui->tableMidiMapping->rowCount();
            addMidiControl(row, mapping.value("channel").toUInt(), mapping.value("control").toUInt(), mapping.value("role").toString());
        }
        mapping.endArray();
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "unable to parse file:" << file;
    }
}

void ConfigDialog::saveMidiMappingFile(QString file)
{
    QSettings mapping(file, QSettings::IniFormat);

    // General
    mapping.beginGroup("general");
    if(ui->leVendor->text() == QString(""))
        ui->leVendor->setText("undefined");
    mapping.setValue("vendor", ui->leVendor->text());
    if(ui->leProduct->text() == QString(""))
        ui->leProduct->setText("undefined");
    mapping.setValue("product", ui->leProduct->text());
    mapping.endGroup();

    // MIDI Controls
    mapping.beginWriteArray("midi_controls");
    // Remove all existing midi_controls entries
    mapping.remove("");
    for (int i = 0; i < ui->tableMidiMapping->rowCount(); ++i) {
        // MIDI Controls
        mapping.setArrayIndex(i);
        mapping.setValue("channel", ui->tableMidiMapping->item(i,0)->text());
        mapping.setValue("control", ui->tableMidiMapping->item(i,1)->text());
        const QComboBox* cb = qobject_cast<QComboBox*>(ui->tableMidiMapping->cellWidget(i,2));
        if(cb)
            mapping.setValue("role", cb->currentText());
    }
    mapping.endArray();
    mapping.sync();

    // Reload files list in combobox
    loadMidiMappingFiles();
    // Reselect the current item
    for(int i=0; i<ui->cbMidiMapping->count(); ++i)
    {
        if(ui->cbMidiMapping->itemData(i).toString() == file)
        {
            ui->cbMidiMapping->setCurrentIndex(i);
            break;
        }
    }
}

void ConfigDialog::updateMidiMappingTab()
{
//    qDebug() << Q_FUNC_INFO;
    ui->tableMidiMapping->clear();
    ui->tableMidiMapping->setColumnCount(4);
    ui->tableMidiMapping->setRowCount(0);
    //Set Header Label Texts
    ui->tableMidiMapping->setHorizontalHeaderLabels(QString("Channel;Control;Role;Value").split(";"));
    loadMidiMappingFiles();
}

void ConfigDialog::updateSerialTab()
{
//    qDebug() << Q_FUNC_INFO;
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
            qDebug() << Q_FUNC_INFO
                     << "Unable to connect to serial port";
        }
    } else {
        Minotor::minotor()->ledMatrix()->closePort();
    }
}

void ConfigDialog::configDialogFinished(int result)
{
    qDebug() << Q_FUNC_INFO
             << "ConfigDialog: result" << result;
    disconnect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
}

void ConfigDialog::on_pbSaveAs_clicked()
{
    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), dataPath, tr("MinoMidiMapping (*.ini)"));
    if(!fileName.isEmpty())
    {
        saveMidiMappingFile(fileName);
    }
}

void ConfigDialog::on_cbMidiMapping_currentIndexChanged(int index)
{
    QVariant filename = ui->cbMidiMapping->itemData(index);
    if(filename.isValid())
    {
        loadMidiMappingFile(filename.toString());
    }
}

void ConfigDialog::on_pushButton_toggled(bool checked)
{
    if(checked)
        connect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)),Qt::UniqueConnection);
    else
        disconnect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
}
