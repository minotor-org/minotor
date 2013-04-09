#include "minopropertyreal.h"

#include <QDebug>

MinoPropertyReal::MinoPropertyReal(QObject *parent) :
    MinoProperty(parent)
{
    _mcr = new MidiControllableReal(this);
    connect(_mcr, SIGNAL(valueChanged(qreal)), this, SIGNAL(valueChanged(qreal)));
}

void MinoPropertyReal::setLabel(const QString &label)
{
    _mcr->setLabel(label);
    MinoProperty::setLabel(label);
}
