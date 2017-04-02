/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * Copyright 2015, 2017 Michiel Brink
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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QFileInfo>
#include <QString>
#include <QComboBox>
#include <QSignalMapper>
#include <QListWidget>


namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    
    void loadMidiMappingFiles(QComboBox *cb);

    void loadMidiMappingEditor();

private slots:
    void midiControlChanged(const int interface, const quint8 channel, const quint8 control, const quint8 value);

    void on_tabWidget_currentChanged(int index);

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pbSerialConnect_clicked(bool checked);

    void on_pbArtnet_clicked(bool checked);
    
    void configDialogFinished(int result);

    void on_pbSaveAs_clicked();

    void on_lwMappings_currentItemChanged();

    void updateMidiInterfaces();
    void midiLearnToggled(const QString& portName);

    void refreshMatrixConfig();

private:
    Ui::ConfigDialog *ui;
    QSignalMapper *_smMidiMappingLearnMapper;
    QStringList _slMidiMappingLearnPorts;

    void addMidiControl(const int row, const quint8 channel, const quint8 control, const QString &role, const quint8 value = 255);

    void updateGeneralTab();
    void updateMidiTab();
    void updateMidiMappingTab();
    void updateSerialTab();
    void updateArtnetTab();

    QTimer *_midiAutoRefreshTimer;
    void addMidiMappingEntry(QFileInfo file, QComboBox *cb);
    void addMidiMappingEditorEntry(QFileInfo file);
    void saveMidiMappingFile(QString file);
};

#endif // CONFIGDIALOG_H
