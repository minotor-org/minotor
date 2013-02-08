#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>

#include "configdialog.h"
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
    // Request from widgets
    void customContextMenuRequested(const QPoint &pos);

    // Action from MIDI menu
    void midiCaptureTrigged();

    // Action from MIDI learn button (toolbar)
    void tbMidiLearnToggled(bool checked);

    // UI: Action file->configuration
    void on_action_Configuration_triggered();

    void tbFullScreenToggled(bool on);

    // Debug: PPQN slider
    void on_sPpqn_valueChanged(int value);


    void on_pbShot_clicked();

    void on_pbScene_clicked();

private:
    // UI
    // == Main window ==
    Ui::MainWindow *ui;
    QAction *_actionMidiCapture;
    QMenu _menu;
    // Channels
    UiMaster *_uiMaster;

   // UiProgramEditor *_uiCueEditor;
    UiProgramEditor *_uiMasterEditor;

    // == Configuration dialog ==
    ConfigDialog *_configDialog;

    // Minotor
    Minotor *_minotor;
};

#endif // MAINWINDOW_H
