#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    
private slots:
    void midiControlChanged(const int interface, const quint8 channel, const quint8 control, const quint8 value);

    void on_tabWidget_currentChanged(int index);

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pbSerialConnect_clicked(bool checked);

    void configDialogFinished(int result);

private:
    Ui::ConfigDialog *ui;

    void addMidiControl(const int row, const int interface, const quint8 channel, const quint8 control, const quint8 value);

};

#endif // CONFIGDIALOG_H
