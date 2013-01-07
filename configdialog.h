#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include "ledmatrix.h"
#include "midi.h"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(LedMatrix *ledMatrix, Midi *midi, QWidget *parent = 0);
    ~ConfigDialog();
    
private slots:
    void on_tabWidget_currentChanged(int index);

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pbSerialConnect_clicked(bool checked);

    void on_pbMidiConnect_clicked(bool checked);

private:
    Ui::ConfigDialog *ui;

    LedMatrix *_ledMatrix;
    Midi *_midi;
};

#endif // CONFIGDIALOG_H
