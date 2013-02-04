#include "uidial.h"

#include <QDebug>
#include <QMotifStyle>

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
    setStyle(new QMotifStyle);

    if(property->step() != 0.0)
    {
        setNotchesVisible(true);
        setSingleStep((qreal)127*property->step());
        qDebug() << "UiDial should display notches (" << singleStep() << ")";
    }
}

void UiDial::setValueFromProperty(qreal value)
{
    this->setValue(value*127);
}

void UiDial::setAnimationPropertyValue(int value)
{
    _property->setValue(((qreal)value)/127);
}
