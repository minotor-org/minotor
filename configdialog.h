#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QSettings>
#include <QFileInfo>
#include <QString>
#include <QComboBox>
#include <QSignalMapper>


namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();
    
    void loadMidiMappingFiles(QComboBox *cb);

private slots:
    void midiControlChanged(const int interface, const quint8 channel, const quint8 control, const quint8 value);

    void on_tabWidget_currentChanged(int index);

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pbSerialConnect_clicked(bool checked);

    void configDialogFinished(int result);

    void on_pbSaveAs_clicked();

    void on_cbMidiMapping_currentIndexChanged(int index);

    void midiLearnToggled(const QString& portName);

private:
    Ui::ConfigDialog *ui;
    QSignalMapper *_smMidiMappingLearnMapper;
    QStringList _slMidiMappingLearnPorts;

    void addMidiControl(const int row, const quint8 channel, const quint8 control, const QString &role, const quint8 value = 255);

    void setupLedMatrix(QSettings &settings);
    void setupMidi(QSettings &settings);

    void updateMidiTab();
    void updateMidiMappingTab();
    void updateSerialTab();

    void addMidiMappingEntry(QFileInfo file, QComboBox *cb);
    void saveMidiMappingFile(QString file);
};

#endif // CONFIGDIALOG_H
