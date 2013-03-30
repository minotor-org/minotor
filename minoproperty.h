#ifndef MINOPROPERTY_H
#define MINOPROPERTY_H

#include <QObject>
#include <QVariant>

#include "midicontrol.h"

class MinoProperty : public QObject
{
    Q_OBJECT
public:
    explicit MinoProperty(QObject *parent);

protected:
};

typedef QList<MinoProperty*> MinoProperties;

#endif // MINOPROPERTY_H
