#ifndef MINOPROPERTYREAL_H
#define MINOPROPERTYREAL_H

#include "minoproperty.h"
#include "midicontrollablereal.h"

class MinoPropertyReal : public MinoProperty
{
    Q_OBJECT
public:
    explicit MinoPropertyReal(QObject *parent);
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
