#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QTime>
#include <QTimer>

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

    void on_pbTransportStart_clicked();

    void on_pbTransportTapping_clicked();

private:
    // UI
    // == Main window ==
    Ui::MainWindow *ui;
    QAction *_actionMidiCapture;
    QMenu _menu;
    // Channels
    UiChannel *_uiChannel1;
    UiChannel *_uiChannel2;
    UiMaster *_uiMaster;

    // == Configuration dialog ==
    ConfigDialog *_configDialog;


    // External connections
    LedMatrix *_ledMatrix;
    Midi *_midi;

    // Minotor
    Minotor *_minotor;

    // BPM tap
    QTime _bpmTap;
    int _bpmValues[10]; // Array used for average
    int _bpmValuesCount;
    int _bpmValuesIndex;
    qreal _bpmAverageMs;

    // Timer
    QTimer _tInternalClockGenerator;
};

#endif // MAINWINDOW_H
