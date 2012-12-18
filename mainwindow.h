#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPropertyAnimation>

#include "ledmatrix.h"

#include "RtMidi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void midiCallback( double deltatime, std::vector< unsigned char > *message);

private slots:
    void on_pushButton_clicked();

    void on_horizontalSliderRed_valueChanged(int value);

    void on_horizontalSliderGreen_valueChanged(int value);

    void on_horizontalSliderBlue_valueChanged(int value);

    void on_pbConnectMidi_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    void midiConnect(unsigned int portIndex);

    void handleClock();
    void handleStop();
    void handleStart();
    void handleContinue();

    Ui::MainWindow *ui;

    LedMatrix * _ledMatrix;
    QGraphicsScene _scene;

    RtMidiIn * _midiIn;
    QPropertyAnimation animation;
};

#endif // MAINWINDOW_H
