#include "minocontrol.h"

MinoControl::MinoControl(QString role, QObject *parent) :
    QObject(parent),
    _value(0),
    _role(role)
{
}

void MinoControl::setValue(quint8 value)
{
    if(value != _value)
    {
        _value = value;
        emit valueChanged(value);
    }
}
