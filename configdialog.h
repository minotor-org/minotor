#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include "minotor.h"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(Minotor *minotor, QWidget *parent = 0);
    ~ConfigDialog();
    
private slots:
    void on_tabWidget_currentChanged(int index);

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pbSerialConnect_clicked(bool checked);

    void on_pbMidiConnect_clicked(bool checked);

private:
    Ui::ConfigDialog *ui;

    Minotor *_minotor;
};

#endif // CONFIGDIALOG_H
