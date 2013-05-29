#ifndef MINOPROPERTYEASINGCURVE_H
#define MINOPROPERTYEASINGCURVE_H

#include "minoproperty.h"
#include "midicontrollablelist.h"

#include <QEasingCurve>
#include <QMetaEnum>

class MinoPropertyEasingCurve : public MinoProperty
{
    Q_OBJECT
    Q_PROPERTY(QEasingCurve easingcurve READ easingCurve WRITE setEasingCurve STORED true)
public:
    explicit MinoPropertyEasingCurve(QObject *parent, bool autofilled = false);
    ~MinoPropertyEasingCurve();

    // Label: Propagate its label to _mcl
    void setLabel(const QString &name);
    void setObjectName(const QString &name);

    // EasingCurveType
    QEasingCurve::Type easingCurveType();
    void setEasingCurveType(const QEasingCurve::Type& type);
    void setEasingCurve(const QEasingCurve ec) { _easingCurveType = ec.type(); }
    QEasingCurve easingCurve() { return QEasingCurve(_easingCurveType); }
    // Add
    void addEasingCurveType(const QEasingCurve::Type& type);

    void setPreferred(bool on = true) { _mcl->setPreferred(on); }

signals:
    void itemChanged(QString name);
    void itemIdChanged(int id);
    void easingCurveChanged(const QEasingCurve& ec);

private:
    MidiControllableList *_mcl;
    QEasingCurve::Type _easingCurveType;

    QString easingCurveTypeToString(const QEasingCurve::Type& type);
    void addAllEasingCurveTypes();

private slots:
    void updateTypeFromValue(qreal value);

};

#endif // MINOPROPERTYEASINGCURVE_H
