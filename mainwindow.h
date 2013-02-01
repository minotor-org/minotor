#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>

#include "configdialog.h"
#include "uichanneleditor.h"
#include "uimaster.h"
#include "uicue.h"

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
    void on_action_Configuration_triggered();
    void midiCaptureTrigged();
    void customContextMenuRequested(const QPoint &pos);

    void on_sPpqn_valueChanged(int value);

    void on_pushButton_toggled(bool checked);

private:
    // UI
    // == Main window ==
    Ui::MainWindow *ui;
    QAction *_actionMidiCapture;
    QMenu _menu;
    // Channels
    UiCue *_uiCue;
    UiMaster *_uiMaster;

    UiChannelEditor *_uiCueEditor;
    UiChannelEditor *_uiMasterEditor;

    // == Configuration dialog ==
    ConfigDialog *_configDialog;

    // Minotor
    Minotor *_minotor;
};

#endif // MAINWINDOW_H
