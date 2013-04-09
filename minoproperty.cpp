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
    }
    QObject::setObjectName(name);
}
