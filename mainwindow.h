#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

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

private slots:
    void on_pushButton_clicked();

    void on_horizontalSliderRed_valueChanged(int value);

    void on_horizontalSliderGreen_valueChanged(int value);

    void on_horizontalSliderBlue_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_pbConnectMidi_clicked();

    void on_pushButton_3_clicked();

private:
    void midiConnect(unsigned int portIndex);
    Ui::MainWindow *ui;

    LedMatrix * _ledMatrix;
    QGraphicsScene _scene;

    RtMidiIn * _midiIn;
};

#endif // MAINWINDOW_H
