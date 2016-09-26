/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

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

    // Create userdata's folder
    QString dataPath = Minotor::dataPath();
    QDir dataDir(dataPath);
    if (!dataDir.exists())
    {
        dataDir.mkpath(dataPath);
    }

    LedMatrix *matrix = Minotor::minotor()->ledMatrix();
    connect(matrix, SIGNAL(connected(bool)), ui->pbSerialConnect, SLOT(setChecked(bool)));
    connect(matrix, SIGNAL(connected(bool)), ui->cbSerialPort, SLOT(setDisabled(bool)));

    _smMidiMappingLearnMapper = new QSignalMapper(this);
    connect(_smMidiMappingLearnMapper, SIGNAL(mapped(QString)),this, SLOT(midiLearnToggled(QString)));

    //Matrix config
    connect(ui->sbPanelsInX, SIGNAL(valueChanged(int)), this, SLOT(refreshMatrixConfig()));
    connect(ui->sbPanelsInY, SIGNAL(valueChanged(int)), this, SLOT(refreshMatrixConfig()));
    connect(ui->sbPanelPixelsInX, SIGNAL(valueChanged(int)), this, SLOT(refreshMatrixConfig()));
    connect(ui->sbPanelPixelsInY, SIGNAL(valueChanged(int)), this, SLOT(refreshMatrixConfig()));

    // Hack to refresh list at startup
    _midiAutoRefreshTimer = new QTimer(this);
    updateGeneralTab();
    updateMidiTab();
    updateMidiMappingTab();
    updateSerialTab();
    disconnect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
    ui->tableMidiMapping->setColumnWidth(0,250);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

#define ROW_ROLE    0
#define ROW_CHANNEL 1
#define ROW_CONTROL 2
#define ROW_VALUE   3

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
                    if(channel == ui->tableMidiMapping->item(row, ROW_CHANNEL)->text().toInt()
                            && control == ui->tableMidiMapping->item(row, ROW_CONTROL)->text().toInt())
                    {
                        ui->tableMidiMapping->item(row, ROW_VALUE)->setText(QString::number(value));
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
    ui->tableMidiMapping->insertRow(row);
    QTableWidgetItem *item;
    // Channel
    item = new QTableWidgetItem(QString::number(channel));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableMidiMapping->setItem(row, ROW_CHANNEL, item);
    // Control
    item = new QTableWidgetItem(QString::number(control));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableMidiMapping->setItem(row, ROW_CONTROL, item);
    // Role
    QComboBox *cb = new QComboBox();
    cb->addItem("none");
    foreach(MinoRole *role, MidiMapper::registeredRoles())
        cb->addItem(role->name());

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
    ui->tableMidiMapping->setCellWidget(row, ROW_ROLE, cb);
    // Value
    item = new QTableWidgetItem(QString::number(value));
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui->tableMidiMapping->setItem(row, ROW_VALUE, item);
    ui->lMidiControlsCount->setText(QString::number(ui->tableMidiMapping->rowCount()));
}

void ConfigDialog::on_tabWidget_currentChanged(int index)
{
    disconnect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
    _midiAutoRefreshTimer->stop();

    switch(index)
    {
    case 0:
        updateGeneralTab();
        break;
    case 1: //Midi
        updateMidiTab();
        break;
    case 2: // MIDI mapping
        updateMidiMappingTab();
        break;
    case 3: // Serial
        updateSerialTab();
        break;
    }
}

void ConfigDialog::updateMidiInterfaces()
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

void ConfigDialog::updateMidiTab()
{
    Midi *midi = Minotor::minotor()->midi();
    connect(_midiAutoRefreshTimer, SIGNAL(timeout()), midi, SLOT(scanMidiInterfaces()), Qt::UniqueConnection);
    connect(midi, SIGNAL(updated()), this, SLOT(updateMidiInterfaces()), Qt::UniqueConnection);
    // Scan new interfaces
    midi->scanMidiInterfaces();
    updateMidiInterfaces();
    _midiAutoRefreshTimer->start(500);
}

void ConfigDialog::loadMidiMappingFiles(QComboBox *cb)
{
    cb->clear();
    cb->addItem("no mapping");

    QStringList filters;
    filters << "*.ini" << "*.mcf";

    // Built-in mapping (in Qt ressource file)
    QString resPath = ":/mappings/";
    QDir resDir(resPath);
    foreach(QFileInfo file, resDir.entryInfoList(filters, QDir::Files))
    {
        addMidiMappingEntry(file, cb);
    }

    // User data folder
    QString dataPath = Minotor::dataPath();
    QDir dataDir(dataPath);
    foreach(QFileInfo file, dataDir.entryInfoList(filters, QDir::Files))
    {
        addMidiMappingEntry(file, cb);
    }
}

void ConfigDialog::loadMidiMappingEditor()
{
    ui->wMappingEditor->setVisible(false);
    ui->lwMappings->clear();
    ui->lwMappings->addItem("NEW MAPPING");

    QStringList filters;
    filters << "*.ini" << "*.mcf";

    // Built-in mapping (in Qt ressource file)
    QString resPath = ":/mappings/";
    QDir resDir(resPath);
    foreach(QFileInfo file, resDir.entryInfoList(filters, QDir::Files))
    {
        addMidiMappingEditorEntry(file);
    }

    // User data folder
    QString dataPath = Minotor::dataPath();
    QDir dataDir(dataPath);
    foreach(QFileInfo file, dataDir.entryInfoList(filters, QDir::Files))
    {
        addMidiMappingEditorEntry(file);
    }
}

void ConfigDialog::addMidiMappingEditorEntry(QFileInfo file)
{
    QSettings mapping(file.absoluteFilePath(), QSettings::IniFormat);
    if(QSettings::NoError == mapping.status())
    {
        mapping.beginGroup("properties");
        QString vendor = mapping.value("vendor", "undefined").toString();
        QString product = mapping.value("product", "undefined").toString();
        QString comment = mapping.value("comment", "").toString();
        QString title = QString("%1 - %2").arg(vendor).arg(product);
        if(!comment.isEmpty()) title += QString(" (%1)").arg(comment);
        QListWidgetItem *item = new QListWidgetItem(title);
        item->setData(Qt::ToolTipRole,file.absoluteFilePath());
        ui->lwMappings->addItem(item);
        mapping.endGroup();
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "unable to parse file:" << file.absoluteFilePath();
    }
}

void ConfigDialog::addMidiMappingEntry(QFileInfo file, QComboBox *cb)
{
    QSettings mapping(file.absoluteFilePath(), QSettings::IniFormat);
    if(QSettings::NoError == mapping.status())
    {
        mapping.beginGroup("properties");
        QString vendor = mapping.value("vendor", "undefined").toString();
        QString product = mapping.value("product", QVariant("undefined")).toString();
        QString comment = mapping.value("comment", "").toString();
        QString title = QString("%1 - %2").arg(vendor).arg(product);
        if(!comment.isEmpty()) title += QString(" (%1)").arg(comment);
        cb->addItem(title, file.absoluteFilePath());
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
    MidiMapping *mm = new MidiMapping();

    // Vendor
    QString vendor = ui->leVendor->text();
    if(vendor.isEmpty()) vendor = "undefined";
    mm->setVendor(vendor);

    // Product
    QString product = ui->leProduct->text();
    if(product.isEmpty()) product = "undefined";
    mm->setProduct(product);

    // Comment
    const QString comment = ui->leComment->text();
    mm->setComment(comment);

    // Tracks
    const int tracks = ui->sbTracks->value();
    mm->setTracks(tracks);

    // Knobs per track
    const int knobsPerTrack = ui->sbKnobsPerTrack->value();
    mm->setKnobsPerTrack(knobsPerTrack);

    // Triggers per track
    const int triggersPerTrack = ui->sbTriggersPerTrack->value();
    mm->setTriggersPerTrack(triggersPerTrack);

    // Clock
    const bool clock = ui->pbAcceptSync->isChecked();
    mm->setAcceptClock(clock);

    // CC
    const bool cc = ui->pbAcceptControlChange->isChecked();
    mm->setAcceptControlChange(cc);

    // Note
    const bool note = ui->pbAcceptNotes->isChecked();
    mm->setAcceptNoteChange(note);

    // Program Change
    const bool pc = ui->pbAcceptProgramChange->isChecked();
    mm->setAcceptProgramChange(pc);

    for (int i = 0; i < ui->tableMidiMapping->rowCount(); ++i) {
        // MIDI Controls
        const QComboBox* cb = qobject_cast<QComboBox*>(ui->tableMidiMapping->cellWidget(i,ROW_ROLE));
        Q_ASSERT(cb);
        const QString role = cb->currentText();
        const uint channel = ui->tableMidiMapping->item(i,ROW_CHANNEL)->text().toUInt();
        const uint control = ui->tableMidiMapping->item(i,ROW_CONTROL)->text().toUInt();
        mm->addAssignedMidiControl(role, channel, control);
    }

    MidiMapping::saveToFile(mm, file);
    delete mm;

    // Reload files list
    loadMidiMappingEditor();

    // Reselect the current item
    for(int i=0; i<ui->lwMappings->count(); ++i)
    {
        const QString data = ui->lwMappings->item(i)->data(Qt::ToolTipRole).toString();
        if(data == file)
        {
            ui->lwMappings->setCurrentItem(ui->lwMappings->item(i));
            break;
        }
    }

    Midi *midi = Minotor::minotor()->midi();
    for (int i = 0; i< midi->interfaces().count();i++)
    {
        if (midi->interfaces().at(i)->mapping() == file)
        {
            midi->interfaces().at(i)->setMapping(file);
        }
    }
}

void ConfigDialog::updateMidiMappingTab()
{
    ui->tableMidiMapping->clear();
    ui->tableMidiMapping->setColumnCount(4);
    ui->tableMidiMapping->setRowCount(0);
    //Set Header Label Texts
    ui->tableMidiMapping->setHorizontalHeaderLabels(QString("Role;Channel;Control;Value").split(';'));
    loadMidiMappingEditor();
}

void ConfigDialog::updateSerialTab()
{
    // Port enumeration
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    QStringList portnames;

    foreach (const QextPortInfo info, ports) {
        if (1 || info.physName.startsWith(QLatin1String("/dev/ttyACM")))
        {
//            qDebug() << "port name:"       << info.portName;
//            qDebug() << "friendly name:"   << info.friendName;
//            qDebug() << "physical name:"   << info.physName;
//            qDebug() << "enumerator name:" << info.enumName;
//            qDebug() << "vendor ID:"       << info.vendorID;
//            qDebug() << "product ID:"      << info.productID;
            portnames.append(info.portName);
        }
    }

    int currentItem = -1;
    LedMatrix *matrix = Minotor::minotor()->ledMatrix();
    if (matrix->isConnected())
    {
        if(portnames.contains(matrix->portName()))
        {
            currentItem = portnames.indexOf(matrix->portName());
        }
    } else {
        if(portnames.contains(ui->cbSerialPort->itemText(ui->cbSerialPort->currentIndex())))
        {
            currentItem = portnames.indexOf(ui->cbSerialPort->itemText(ui->cbSerialPort->currentIndex()));
        }
        else
        {
            if (portnames.contains(matrix->portName()))
            {
                currentItem = portnames.indexOf(matrix->portName());
            }
        }
    }

    // Clear combobox
    ui->cbSerialPort->clear();
    ui->cbSerialPort->addItems(portnames);
    ui->cbSerialPort->setCurrentIndex(currentItem);
}

void ConfigDialog::updateGeneralTab()
{
    QSize size = Minotor::minotor()->rendererSize();
    QSize matrixSize = Minotor::minotor()->matrixSize();
    QSize panelSize = Minotor::minotor()->panelSize();
    ui->sbSceneHeight->setValue(size.height());
    ui->sbSceneWidth->setValue(size.width());
    ui->sbPanelsInX->setValue(matrixSize.width());
    ui->sbPanelsInY->setValue(matrixSize.height());
    ui->sbPanelPixelsInX->setValue(panelSize.width());
    ui->sbPanelPixelsInY->setValue(panelSize.height());
}

void ConfigDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Save)
    {
        Minotor::minotor()->setRendererSize(QSize(ui->sbSceneWidth->value(), ui->sbSceneHeight->value()));
        Minotor::minotor()->setMatrixSize(QSize(ui->sbPanelsInX->value(), ui->sbPanelsInY->value()));
        Minotor::minotor()->setPanelSize(QSize(ui->sbPanelPixelsInX->value(), ui->sbPanelPixelsInY->value()));
        Minotor::minotor()->saveSettings();
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
            ui->pbSerialConnect->setChecked(false);
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
    QString dataPath = Minotor::dataPath();
    QString fileName = ui->lwMappings->currentItem()->data(Qt::ToolTipRole).toString();
    // if empty or is a built-in file, show dialog
    if (fileName.isEmpty() || fileName.startsWith(QString(":/")) )
    {
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"), dataPath, tr("MinoMidiMapping (*.ini)"));
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "Current file : " << fileName;
    }
    if(!fileName.isEmpty())
    {
        saveMidiMappingFile(fileName);
    }
}

void ConfigDialog::on_lwMappings_currentItemChanged()
{
    if (ui->lwMappings->currentItem() != NULL)
    {
        ui->wMappingEditor->setVisible(true);
        // User choose a MIDI mapping from combobox
        QVariant filename = ui->lwMappings->currentItem()->data(Qt::ToolTipRole);
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
        ui->leComment->setText(mm->comment());
        ui->sbTracks->setValue(mm->tracks());
        ui->sbKnobsPerTrack->setValue(mm->knobsPerTrack());
        ui->sbTriggersPerTrack->setValue(mm->triggersPerTrack());
        ui->pbAcceptSync->setChecked(mm->acceptClock());
        ui->pbAcceptNotes->setChecked(mm->acceptNoteChange());
        ui->pbAcceptControlChange->setChecked(mm->acceptControlChange());
        ui->pbAcceptProgramChange->setChecked(mm->acceptProgramChange());

        // Add each available assignement (role <-> control)
        QMapIterator<QString, QVariant> i(mm->map());
        while (i.hasNext()) {
            i.next();
            const int row = ui->tableMidiMapping->rowCount();
            const QStringList sl = i.value().toStringList();
            qDebug() << Q_FUNC_INFO
                     << sl;
            Q_ASSERT(sl.count()==3);
            switch (sl.at(0).toUInt())
            {
            case 0: // CC
            {
                addMidiControl(row, sl.at(1).toUInt(), sl.at(2).toUInt(), i.key());
            }
                break;
            case 1: // Note
            {
                // FIXME
                //addMidiControl(row, sl.at(1).toUInt(), sl.at(2).toUInt(), i.key());
            }
                break;
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
                // If its a generic MIDI or the selected mapping match with current MIDI interface
                if(generic_midi_selected || (mi->mapping() == ui->lwMappings->currentItem()->data(Qt::ToolTipRole).toString()))
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
    else
    {
        ui->wMappingEditor->setVisible(false);
        ui->tableMidiMapping->clearContents();
        ui->tableMidiMapping->setRowCount(0);
        ui->leVendor->setText("");
        ui->leProduct->setText("");
        ui->leComment->setText("");
        ui->sbTracks->setValue(0);
        ui->sbKnobsPerTrack->setValue(0);
        ui->sbTriggersPerTrack->setValue(0);
        ui->pbAcceptSync->setChecked(false);
        ui->pbAcceptNotes->setChecked(false);
        ui->pbAcceptControlChange->setChecked(false);
        ui->pbAcceptProgramChange->setChecked(false);
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
        // portName is already in list, means it should be toggled to off
        _slMidiMappingLearnPorts.removeAt(_slMidiMappingLearnPorts.indexOf(portName));
    }
    else
    {
        // register portName as listened ports
        _slMidiMappingLearnPorts.append(portName);
        foreach(MidiInterface* mi, Minotor::minotor()->midi()->interfaces())
        {
            if(mi->portName() == portName) {
                mi->setAcceptControlChange(true);
                mi->setAcceptNoteChange(true);
                break;
            }
        }
    }

    // If there is at least one port in list, connect signals from MIDI manager
    if(_slMidiMappingLearnPorts.count())
        connect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)),Qt::UniqueConnection);
    else
        disconnect(Minotor::minotor()->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
}

void ConfigDialog::refreshMatrixConfig()
{
    QSize panels = QSize(ui->sbPanelsInX->value(),ui->sbPanelsInY->value());
    QSize panelPixels = QSize(ui->sbPanelPixelsInX->value(),ui->sbPanelPixelsInY->value());

    ui->sbSceneWidth->setValue(panels.width()*panelPixels.width());
    ui->sbSceneHeight->setValue(panels.height()*panelPixels.height());
}
