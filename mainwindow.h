#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_pbConnectMidi_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pbConnectSerial_clicked();

private:
    // UI
    Ui::MainWindow *ui;

    // External connections
    LedMatrix *_ledMatrix;
    Midi *_midi;

    // Minotor
    Minotor *_minotor;
};

#endif // MAINWINDOW_H
