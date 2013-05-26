#ifndef UIMIDICONTROLLABLEPARAMETER_H
#define UIMIDICONTROLLABLEPARAMETER_H

#include <QWidget>

#include "midicontrollableparameter.h"

class UiMidiControllableParameter : public QWidget
{
    Q_OBJECT
public:
    explicit UiMidiControllableParameter(MidiControllableParameter *parameter, QWidget *parent, bool editorMode = false);
    void setMidiLearnMode(bool on = true);

protected:
    void paintEvent(QPaintEvent *pe);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    bool _midiLearnMode;
    bool _midiControlled;

    MidiControllableParameter *parameter;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:
    MidiControllableParameter *_parameter;
signals:
    
public slots:
    
private slots:
    void togglePropertyToMaster(bool on);
};

#endif // UIMIDICONTROLLABLEPARAMETER
