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

#include "midimapper.h"
#include "midimapping.h"

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

    _smMidiMappingLearnMapper = new QSignalMapper(this);
    connect(_smMidiMappingLearnMapper, SIGNAL(mapped(QString)),this, SLOT(midiLearnToggled(QString)));

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

        foreach (const QextPortInfo info, ports) {
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
    foreach(const QString& group, settings.childGroups())
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
    for(int i=0;i<Minotor::minotor()->midi()->interfaces().count();++i)
    {
        MidiInterface *mi = Minotor::minotor()->midi()->interfaces().at(i);
        if(mi->id() == interface)
        {
            if(_slMidiMappingLearnPorts.contains(mi->portName()))
            {
                bool found = false;
                int row;
                for(row = 0; row < ui->tableMidiMapping->rowCount(); row++)
                {
                    if(channel == ui->tableMidiMapping->item(row, 0)->text().toInt()
                            && control == ui->tableMidiMapping->item(row, 1)->text().toInt())
                    {
                        ui->tableMidiMapping->item(row, 3)->setText(QString::number(value));
                        ui->tableMidiMapping->selectRow(row);
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    addMidiControl(row, channel, control, "none", value);
                    ui->tableMidiMapping->selectRow(row);
                    row++;
                }
            }
            break;
        }
    }
}

void ConfigDialog::addMidiControl(const int row, const quint8 channel, const quint8 control, const QString& role, const quint8 value)
{
    qDebug() << Q_FUNC_INFO
             << "role" << role;
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
    cb->addItems(MidiMapper::registeredRoles());
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

void ConfigDialog::loadMidiMappingFiles(QComboBox *cb)
{
    cb->clear();
    cb->addItem("Generic MIDI controller");

    QString dataPath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    QDir dataDir(dataPath);
    foreach(QFileInfo file, dataDir.entryInfoList(QDir::Files))
    {
        addMidiMappingEntry(file, cb);
    }
}

void ConfigDialog::addMidiMappingEntry(QFileInfo file, QComboBox *cb)
{
    QSettings mapping(file.absoluteFilePath(), QSettings::IniFormat);
    if(QSettings::NoError == mapping.status())
    {
        mapping.beginGroup("general");
        QString vendor = mapping.value("vendor", "undefined").toString();
        QString product = mapping.value("product", QVariant("undefined")).toString();
        cb->addItem(QString("%1 - %2 (%3)").arg(vendor).arg(product).arg(file.fileName()), file.absoluteFilePath());
        mapping.endGroup();
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "unable to parse file:" << file.absoluteFilePath();
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
    loadMidiMappingFiles(ui->cbMidiMapping);
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
    ui->tableMidiMapping->setHorizontalHeaderLabels(QString("Channel;Control;Role;Value").split(';'));
    loadMidiMappingFiles(ui->cbMidiMapping);
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
    // User choose a MIDI mapping from combobox
    QVariant filename = ui->cbMidiMapping->itemData(index);
    MidiMapping * mm = NULL;

    const bool generic_midi_selected = !filename.isValid();
    if(!generic_midi_selected)
    {
        // Load the selected mapping
        mm = MidiMapping::loadFromFile(filename.toString());
    }
    if(!mm)
    {
        mm = new MidiMapping();
    }

    // Reload MIDI mapping table content
    ui->tableMidiMapping->clearContents();
    ui->tableMidiMapping->setRowCount(0);
    ui->leVendor->setText(mm->vendor());
    ui->leProduct->setText(mm->product());

    // Add each available assignement (role <-> control)
    QMapIterator<QString, QVariant> i(mm->map());
    while (i.hasNext()) {
        i.next();
        const int row = ui->tableMidiMapping->rowCount();
        QRegExp rx("CC(\\d+):(\\d+)");
        if(rx.indexIn(i.value().toString()) == -1)
        {
            qDebug() << Q_FUNC_INFO
                     << "no match for control:" << i.value().toString();
        }
        else
        {
            QStringList sl = rx.capturedTexts();
            qDebug() << Q_FUNC_INFO
                        << "captured texts:" << sl;
            addMidiControl(row, sl.at(1).toUInt(), sl.at(2).toUInt(), i.key());
        }
    }
    delete mm;

    foreach(QPushButton *pb, ui->wMidiMappingBottom->findChildren<QPushButton*>("midi-learn"))
    {
        if(pb->isChecked())
        {
            // If active we need to remove it from our list
            pb->setChecked(false);
        }
        // Delete all available midi-learn related buttons
        delete pb;
    }

    // Create buttons for midi-learn
    Midi *midi = Minotor::minotor()->midi();
    foreach(MidiInterface *mi, midi->interfaces())
    {
        // Draw a pushbutton only when MIDI interface is available (and useful)
        if(mi->isConnected() && mi->isUsed())
        {
            // If its a generic MIDI or the selected mapping matchs with current MIDI interface
            if(generic_midi_selected || (mi->mapping() == ui->cbMidiMapping->currentText()))
            {
                QPushButton *pb = new QPushButton(QString("Learn with \"%1\"").arg(mi->portName()), ui->wMidiMappingBottom);
                pb->setObjectName("midi-learn");
                pb->setCheckable(true);
                connect(pb,SIGNAL(toggled(bool)),_smMidiMappingLearnMapper,SLOT(map()));
                _smMidiMappingLearnMapper->setMapping(pb,QString(mi->portName()));
                QBoxLayout *layout = qobject_cast<QBoxLayout*>(ui->wMidiMappingBottom->layout());
                Q_ASSERT(layout);
                layout->insertWidget(0, pb);
            }
        }
    }
}

void ConfigDialog::midiLearnToggled(const QString &portName)
{
    /*
    qDebug() << Q_FUNC_INFO
             << portName;
             */
    // Toggle MIDI interface learning
    if(_slMidiMappingLearnPorts.contains(portName))
    {
        // portName is already in list, means it should be toogled to off
        _slMidiMappingLearnPorts.removeAt(_slMidiMappingLearnPorts.indexOf(portName));
    }
    else
    {
        // register portName as listened ports
        _slMidiMappingLearnPorts.append(portName);
    }

    // If there is at least one port in list, connect signals from MIDI manager
    if(_slMidiMappingLearnPorts.count())
        connect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)),Qt::UniqueConnection);
    else
        disconnect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
}
