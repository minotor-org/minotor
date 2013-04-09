#include "minopropertyeasingcurve.h"

#include <QMetaObject>
#include <QDebug>

MinoPropertyEasingCurve::MinoPropertyEasingCurve(QObject *parent) :
    MinoProperty(parent)
{
    _mcl = new MidiControllableList(this);

    const QMetaObject mo = QEasingCurve::staticMetaObject;
    int id = mo.indexOfEnumerator("Type");
    if(id!=-1)
    {
        _metaEnum = mo.enumerator(id);
        for(int i=0; i<_metaEnum.keyCount(); ++i)
        {
            _mcl->addItem(QString(_metaEnum.valueToKey(i)), i);
        }
        _mcl->setCurrentItemIndex(0);
    }

    connect(_mcl, SIGNAL(itemChanged(QString)), this, SIGNAL(itemChanged(QString)));
    connect(_mcl, SIGNAL(itemIdChanged(int)), this, SIGNAL(itemIdChanged(int)));
}

MinoPropertyEasingCurve::~MinoPropertyEasingCurve()
{
    delete _mcl;
}

QEasingCurve::Type MinoPropertyEasingCurve::currentType()
{
    const QMetaObject mo = QEasingCurve::staticMetaObject;
    int id = mo.indexOfEnumerator("Type");
    _metaEnum = mo.enumerator(id);
    QString name = _mcl->currentItem()->name();
    id = _metaEnum.keyToValue(name.toAscii());
    return (QEasingCurve::Type)id;
}

void MinoPropertyEasingCurve::setCurrentType(const QEasingCurve::Type &type)
{
    _mcl->setCurrentItemIndex((int)type);
}

void MinoPropertyEasingCurve::setLabel(const QString &label)
{
    _mcl->setLabel(label);
    MinoProperty::setLabel(label);
}
