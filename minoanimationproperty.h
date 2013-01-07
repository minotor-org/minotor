#ifndef MINOANIMATIONPROPERTY_H
#define MINOANIMATIONPROPERTY_H

#include <QObject>

class MinoAnimationProperty : public QObject
{
    Q_OBJECT
public:
    explicit MinoAnimationProperty(qreal value = 0.0, QObject *parent = 0);
    qreal value();
signals:
    void valueChanged(qreal value);
public slots:
    void midiControlValueChange(quint8 value);
protected:
    qreal _value;
};

typedef QList<MinoAnimationProperty*> MinoAnimationPropertyList;

#endif // MINOANIMATIONPROPERTY_H
