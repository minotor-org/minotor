#include "minotrigger.h"

MinoTrigger::MinoTrigger(QString role, QObject *parent) :
    QObject(parent),
    _on(false),
    _role(role)
{
}

void MinoTrigger::setStatus(bool on)
{
    if(on != _on)
    {
        _on = on;
        emit toogled(on);
        if(on)
            emit triggered();
    }
}
