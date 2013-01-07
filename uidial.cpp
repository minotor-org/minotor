#include "uidial.h"

UiDial::UiDial(MinoAnimationProperty *property, QWidget *parent) :
    QDial(parent),
    _property(property)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(property, SIGNAL(valueChanged(qreal)), this, SLOT(setValueFromProperty(qreal)));
    this->setValueFromProperty(property->value());
    this->setMaximum(127);
    this->setMinimum(0);
}

void UiDial::setValueFromProperty(qreal value)
{
    this->setValue(value*127);
}
