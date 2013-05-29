#ifndef MINOPROPERTYREAL_H
#define MINOPROPERTYREAL_H

#include "minoproperty.h"
#include "midicontrollablereal.h"

class MinoPropertyReal : public MinoProperty
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ value WRITE setValue STORED true)
public:
    explicit MinoPropertyReal(QObject *parent);

    // Label: Propagate label name to _mcr
    void setLabel(const QString &label);
    void setObjectName(const QString &name);

    // value (0.0 - 1.0)
    qreal value() { return _mcr->value(); }
    void setValue(qreal value) { return _mcr->setValue(value); }
    void setPreferred(bool on = true) { _mcr->setPreferred(on); }

protected slots:

private:
    MidiControllableReal *_mcr;

signals:
    void valueChanged(qreal);

public slots:
    
};

#endif // MINOPROPERTYREAL_H
