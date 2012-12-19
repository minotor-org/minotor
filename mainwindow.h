#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPropertyAnimation>

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
    // Midi messages handlers
    void handleClock();
    void handleStop();
    void handleStart();
    void handleContinue();

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

    // Animations
    QGraphicsScene _scene;
    QPropertyAnimation animation;

    // Sequence watching
    unsigned int _ppqnId;    //pulse per quarter note's ID (ie. 0 -> ... -> 23 -> 0) Note: a "start" midi message resets this counter
    bool _isSequenceRunning; //sets when midi device is in "running mode" (ie. after "start" or "continue" midi message)

};

#endif // MAINWINDOW_H
