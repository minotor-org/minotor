#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "configdialog.h"

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
    ~MainWindow();

private slots:
    // UI related slots
    void on_pushButton_clicked();

    void on_horizontalSliderRed_valueChanged(int value);

    void on_horizontalSliderGreen_valueChanged(int value);

    void on_horizontalSliderBlue_valueChanged(int value);

    void on_action_Configuration_triggered();

    void customContextMenuRequested(const QPoint &pos);

    void midiCaptureTrigged();

private:
    // UI
    Ui::MainWindow *ui;
    ConfigDialog *_configDialog;
    QAction *_midiCaptureAction;

    // External connections
    LedMatrix *_ledMatrix;
    Midi *_midi;

    // Minotor
    Minotor *_minotor;
};

#endif // MAINWINDOW_H
