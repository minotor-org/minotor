#ifndef MIDICONTROL_H
#define MIDICONTROL_H

#include <QObject>

class MidiControl : public QObject
{
    Q_OBJECT
public:
    explicit MidiControl(int interface, quint8 channel, quint8 control, QObject *parent = 0);
    void setValue(quint8 value);
    int interface() const { return _interface; }
    quint8 channel() const { return _channel; }
    quint8 control() const { return _control; }
    quint8 value() const { return _value; }

protected:
    quint8 _value;
    int _interface;
    quint8 _channel;
    quint8 _control;

signals:
    void valueChanged(quint8 value);

public slots:
    
};

typedef QList<MidiControl*> MidiControlList;

#endif // MIDICONTROL_H
