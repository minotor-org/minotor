#include "uidial.h"

#include <QDebug>
#include <QMotifStyle>

#include "qnumeric.h"

UiDial::UiDial(MinoProperty *property, QWidget *parent) :
    QDial(parent),
    _property(property)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setFocusPolicy(Qt::NoFocus);    
    this->setMaximumHeight(30);
    //this->setStyle(new QMotifStyle());

    if(_property->step() != 0.0)
    {
        setNotchesVisible(true);
        const qreal step = (qreal)127*_property->step();
        setSingleStep(qRound(step));
        //qDebug() << "UiDial should display notches (" << singleStep() << "~=" << step << ")";
    }
    connect(_property, SIGNAL(valueChanged(qreal)), this, SLOT(setValueFromProperty(qreal)));
    this->setMinimum(0);
    this->setMaximum(127);
    this->setValueFromProperty(_property->value());
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(setAnimationPropertyValue(int)));
}

void UiDial::setValueFromProperty(qreal value)
{
    disconnect(_property, SIGNAL(valueChanged(qreal)), this, SLOT(setValueFromProperty(qreal)));
    this->blockSignals(true);
    const int lvalue = (qreal)value*127;
    this->setValue(lvalue);
    this->blockSignals(false);
    connect(_property, SIGNAL(valueChanged(qreal)), this, SLOT(setValueFromProperty(qreal)));
}

void UiDial::setAnimationPropertyValue(int value)
{
    _property->setValue(((qreal)value)/127);
}
