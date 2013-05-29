#include "easingcurvedreal.h"

EasingCurvedReal::EasingCurvedReal(QObject *parent) :
    QObject(parent),
    _start(0.0),
    _end(1.0),
    _ec(QEasingCurve::Linear)
{
}

