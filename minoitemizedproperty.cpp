#include "minoitemizedproperty.h"

#include <QDebug>

MinoItemizedProperty::MinoItemizedProperty(QObject *parent):
    MinoProperty(parent)
{
    _mcl = new MidiControllableList(this);
    connect(_mcl, SIGNAL(itemChanged(QString)), this, SIGNAL(itemChanged(QString)));
    connect(_mcl, SIGNAL(itemIdChanged(int)), this, SIGNAL(itemIdChanged(int)));
}

void MinoItemizedProperty::addItem(const QString name, const qreal real)
{
    _mcl->addItem(name, real);
}

MidiControllableListItem *MinoItemizedProperty::currentItem()
{
    return _mcl->currentItem();
}

void MinoItemizedProperty::setCurrentItemIndex(int index)
{
    _mcl->setCurrentItemIndex(index);
}

void MinoItemizedProperty::setCurrentItem(const QString name)
{
    _mcl->setCurrentItem(name);
}

void MinoItemizedProperty::setLinear(bool linear)
{
    _mcl->setLinear(linear);
}

void MinoItemizedProperty::setObjectName(const QString &name)
{
    _mcl->setObjectName(name);
    QObject::setObjectName(name);
}
