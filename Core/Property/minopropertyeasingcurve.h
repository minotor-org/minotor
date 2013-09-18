/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    void setEasingCurve(const QEasingCurve ec);
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
