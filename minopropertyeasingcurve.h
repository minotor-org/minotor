#ifndef MINOPROPERTYEASINGCURVE_H
#define MINOPROPERTYEASINGCURVE_H

#include "minoproperty.h"
#include "midicontrollablelist.h"

#include <QEasingCurve>
#include <QMetaEnum>

class MinoPropertyEasingCurve : public MinoProperty
{
    Q_OBJECT
public:
    explicit MinoPropertyEasingCurve(QObject *parent);
    ~MinoPropertyEasingCurve();

    // Label: Propagate its label to _mcl
    void setLabel(const QString &name);

    // Items
    QEasingCurve::Type currentType();
    void setCurrentType(const QEasingCurve::Type &type);
    void setPreferred(bool on = true) { _mcl->setPreferred(on); }

signals:
    void itemChanged(QString name);
    void itemIdChanged(int id);

private:
    MidiControllableList *_mcl;
    QMetaEnum _metaEnum;

signals:
    
public slots:
    
};

#endif // MINOPROPERTYEASINGCURVE_H
