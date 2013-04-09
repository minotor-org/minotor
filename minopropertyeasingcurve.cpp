#include "minopropertyeasingcurve.h"

#include <QMetaObject>
#include <QDebug>

MinoPropertyEasingCurve::MinoPropertyEasingCurve(QObject *parent, bool autofilled) :
    MinoProperty(parent)
{
    _mcl = new MidiControllableList(this);

    setObjectName("easingcurve");
    setLabel("Curve");

    if(autofilled)
        addAllEasingCurveTypes();

    connect(_mcl, SIGNAL(itemChanged(QString)), this, SIGNAL(itemChanged(QString)));
    connect(_mcl, SIGNAL(itemIdChanged(int)), this, SIGNAL(itemIdChanged(int)));
    connect(_mcl, SIGNAL(itemValueChanged(qreal)), this, SLOT(updateTypeFromValue(qreal)));
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
            addEasingCurveType((QEasingCurve::Type)metaEnum.value(i));
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
    qDebug() << Q_FUNC_INFO
             << value
             << valueAsType
             << easingCurveTypeToString(valueAsType);
    if(_easingCurveType != valueAsType)
    {
        setEasingCurveType(valueAsType);
    }
}
