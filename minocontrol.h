#ifndef MINOCONTROL_H
#define MINOCONTROL_H

#include <QObject>

class MinoControl : public QObject
{
    Q_OBJECT
public:
    explicit MinoControl(QString role, QObject *parent = 0);
    
    void setValue(quint8 value);

    QString role() { return _role; }
signals:
    void valueChanged(quint8);

public slots:
    
protected:
    quint8 _value;
    QString _role;
};

#endif // MINOCONTROL_H
