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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>

#include "configdialog.h"
#include "externalmasterview.h"
#include "uiprogrameditor.h"
#include "uimaster.h"

#include "minotor.h"
#include "ledmatrix.h"
#include "midi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    QAction* midiCaptureAction();
    ~MainWindow();

private slots:
    // Action from MIDI learn button (toolbar)
    void tbMidiLearnToggled(bool checked);

    void pbClockSourceChecked(bool checked);

    // UI: Action file->configuration
    void on_action_Configuration_triggered();
    // UI: Action help->wiki
    void on_action_MinotorWiki_triggered();
    // UI: File Quit
    void on_actionQuit_triggered();
    // UI: Program Bank/Load
    void on_actionLoad_triggered();
    // UI: Program Bank/Save
    void on_actionSave_triggered();
    // UI: Program Bank/SaveAs
    void on_actionSaveAs_triggered();
    // UI: Add new program to bank
    void on_actionNewProgram_triggered();
    // UI: Import Program
    void on_actionImport_triggered();

    void tbFullScreenToggled(bool on);
    void tbViewmodeToggled(bool on);

    // Debug: PPQN slider
    void on_sPpqn_valueChanged(int value);


    void on_pbShot_clicked();

    void on_pbScene_clicked();

    void beatToggledReceived(bool active);
    void midiDataReceived();

    void on_actionExternal_master_view_toggled(bool on);

    void on_pbSave_clicked();

    void on_pbLoad_clicked();

    void on_actionNew_triggered();

    void createUiProgramBank(QObject *bank);

private:
    // UI
    // == Main window ==
    Ui::MainWindow *ui;

    // Master
    UiMaster *_uiMaster;
    UiProgramEditor *_uiMasterEditor;

    // Animation toolbar
    QToolBar *_tAnimationToolBar;

    // Configuration dialog
    ConfigDialog *_configDialog;

    // Minotor
    Minotor *_minotor;

    QWidget *_wClockLed;
    QWidget *_wMidiLed;

    // External master view
    ExternalMasterView *_externalMasterView;

    // Current ProgramBankFile
    QString _programBankFileName;

    // Timer for MIDI data led
    // FIXME: Wrote a dedicated QWidget (performance)
    QBasicTimer _midiDataLedTimer;

    QPushButton *_pbClockSource;

    void timerEvent(QTimerEvent *);

    void setMidiDataLedStatus(bool active);
};

#endif // MAINWINDOW_H
