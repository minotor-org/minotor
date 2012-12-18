#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
