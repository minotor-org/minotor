#include "uidial.h"

#include <QDebug>

UiDial::UiDial(MinoAnimationProperty *property, QWidget *parent) :
    QDial(parent),
    _property(property)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(property, SIGNAL(valueChanged(qreal)), this, SLOT(setValueFromProperty(qreal)));
    this->setValueFromProperty(property->value());
    this->setMaximum(127);
    this->setMinimum(0);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(setAnimationPropertyValue(int)));
    this->setMaximumHeight(40);
}

void UiDial::setValueFromProperty(qreal value)
{
    this->setValue(value*127);
}

void UiDial::setAnimationPropertyValue(int value)
{
    _property->setValue(((qreal)value)/127);
}
