/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "minopropertyeasingcurve.h"

#include <QMetaObject>
#include <QDebug>

MinoPropertyEasingCurve::MinoPropertyEasingCurve(QObject *parent, bool autofilled) :
    MinoProperty(parent),
    _easingCurveType(QEasingCurve::Linear)
{
    _mcl = new MidiControllableList(this);

    setLabel("Curve");

    connect(_mcl, SIGNAL(itemValueChanged(qreal)), this, SLOT(updateTypeFromValue(qreal)));

    if(autofilled)
        addAllEasingCurveTypes();

    connect(_mcl, SIGNAL(itemChanged(QString)), this, SIGNAL(itemChanged(QString)));
    connect(_mcl, SIGNAL(itemIdChanged(int)), this, SIGNAL(itemIdChanged(int)));
}

MinoPropertyEasingCurve::~MinoPropertyEasingCurve()
{
    delete _mcl;
}

QEasingCurve::Type MinoPropertyEasingCurve::easingCurveType()
{
    return _easingCurveType;
}

void MinoPropertyEasingCurve::addEasingCurveType(const QEasingCurve::Type &type)
{
    _mcl->addItem(easingCurveTypeToString(type), (int)type);
}

void MinoPropertyEasingCurve::addAllEasingCurveTypes()
{
    const QMetaObject mo = QEasingCurve::staticMetaObject;
    int id = mo.indexOfEnumerator("Type");
    if(id!=-1)
    {
        QMetaEnum metaEnum = mo.enumerator(id);
        for(int i=0; i<metaEnum.keyCount(); ++i)
        {
            const QEasingCurve::Type type = (QEasingCurve::Type)metaEnum.value(i);
            if((type != QEasingCurve::Custom)
                    && (type != QEasingCurve::NCurveTypes))
            {
                addEasingCurveType(type);
            }
        }
        _mcl->setCurrentItemIndex(0);
    }
}

QString MinoPropertyEasingCurve::easingCurveTypeToString(const QEasingCurve::Type& type)
{
    QMetaEnum metaEnum = QEasingCurve::staticMetaObject.enumerator(QEasingCurve::staticMetaObject.indexOfEnumerator("Type"));
    Q_ASSERT(metaEnum.isValid());
    QString s(metaEnum.valueToKey(type));
    return s;
}

void MinoPropertyEasingCurve::setEasingCurveType(const QEasingCurve::Type& type)
{
    if(_easingCurveType != type)
    {
        _easingCurveType = type;
        _mcl->setCurrentItem(easingCurveTypeToString(type));
        emit easingCurveChanged(QEasingCurve(type));
    }
}

void MinoPropertyEasingCurve::setLabel(const QString &label)
{
    _mcl->setLabel(label);
    MinoProperty::setLabel(label);
}

void MinoPropertyEasingCurve::updateTypeFromValue(qreal value)
{
    QEasingCurve::Type valueAsType = (QEasingCurve::Type)(int)value;
    if(_easingCurveType != valueAsType)
    {
        setEasingCurveType(valueAsType);
    }
}

void MinoPropertyEasingCurve::setObjectName(const QString &name)
{
    _mcl->setObjectName(name);
    MinoProperty::setObjectName(name);
}
