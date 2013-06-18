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

#include "minoproperty.h"

#include <QDebug>

MinoProperty::MinoProperty(QObject *parent) :
    MinoPersistentObject(parent)
{
}

void MinoProperty::setObjectName(const QString &name)
{
    if(name.contains(QChar('.'))
            || name.contains(QChar(' '))
            || (name.toLower() != name ))
    {
        qDebug() << Q_FUNC_INFO
                 << "objectName should not contains upper letters nor dots nor whitespaces";
        qDebug() << "    please fix this object:" << this << "with objectName:" << name;
        Q_ASSERT(false);
    }
    QObject::setObjectName(name);
}
