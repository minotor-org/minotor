#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>

#include "configdialog.h"
#include "uichannel.h"
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
    void on_action_Configuration_triggered();
    void midiCaptureTrigged();
    void customContextMenuRequested(const QPoint &pos);

    void on_sPpqn_valueChanged(int value);

private:
    // UI
    // == Main window ==
    Ui::MainWindow *ui;
    QAction *_actionMidiCapture;
    QMenu _menu;
    // Channels
    UiChannel *_uiCue;
    UiMaster *_uiMaster;

    // == Configuration dialog ==
    ConfigDialog *_configDialog;

    // Minotor
    Minotor *_minotor;
};

#endif // MAINWINDOW_H
