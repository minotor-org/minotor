#include "minopropertyreal.h"

#include <QDebug>

MinoPropertyReal::MinoPropertyReal(QObject *parent) :
    MinoProperty(parent)
{
    _mcr = new MidiControllableReal(this);
    connect(_mcr, SIGNAL(valueChanged(qreal)), this, SIGNAL(valueChanged(qreal)));
}

void MinoPropertyReal::setObjectName(const QString &name)
{
    _mcr->setObjectName(name);
    QObject::setObjectName(name);
}
