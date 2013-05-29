#ifndef MINOPROPERTY_H
#define MINOPROPERTY_H

#include <QObject>
#include <QVariant>

#include "minopersistentobject.h"

class MinoProperty : public MinoPersistentObject
{
    Q_OBJECT
public:
    explicit MinoProperty(QObject *parent);
    void setObjectName(const QString &name);

    void setLabel(const QString &label) { _label = label; }
    QString label() { return _label; }

protected:

private:
    QString _label;
};

typedef QList<MinoProperty*> MinoProperties;

#endif // MINOPROPERTY_H
