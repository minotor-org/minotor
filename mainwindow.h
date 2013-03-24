#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>

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
    // Action from MIDI menu
    void midiCaptureTrigged();

    // Action from MIDI learn button (toolbar)
    void tbMidiLearnToggled(bool checked);

    // UI: Action file->configuration
    void on_action_Configuration_triggered();
    // UI: Action help->wiki
    void on_action_MinotorWiki_triggered();

    void tbFullScreenToggled(bool on);
    void tbViewmodeToggled(bool on);

    // Debug: PPQN slider
    void on_sPpqn_valueChanged(int value);


    void on_pbShot_clicked();

    void on_pbScene_clicked();

    void beatToggledReceived(bool active);

    void on_actionExternal_master_view_toggled(bool on);

    void on_pbSave_clicked();

private:
    // UI
    // == Main window ==
    Ui::MainWindow *ui;
    QAction *_actionMidiCapture;
    QMenu _menu;

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

    // External master view
    ExternalMasterView *_externalMasterView;
};

#endif // MAINWINDOW_H
