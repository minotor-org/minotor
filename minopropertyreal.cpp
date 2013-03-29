#include "minopropertyreal.h"

MinoPropertyReal::MinoPropertyReal(QObject *parent) :
    MinoProperty(parent)
{
    _mcr = new MidiControllableReal(this);
    connect(_mcr, SIGNAL(valueChanged(qreal)), this, SIGNAL(valueChanged(qreal)));
}
