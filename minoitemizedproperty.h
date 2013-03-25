#ifndef MINOITEMIZEDPROPERTY_H
#define MINOITEMIZEDPROPERTY_H

#include "minomidicontrolableproperty.h"

class MinoItemizedPropertyItem
{
public:
    explicit MinoItemizedPropertyItem(const QString name, const qreal real):
        _name(name),
        _real(real)
    {}

    QString name() { return _name; }
    qreal real() { return _real; }

private:
    QString _name;
    qreal _real;
};

class MinoItemizedProperty : public MinoMidiControlableProperty
{
    Q_OBJECT
public:
    explicit MinoItemizedProperty();
    ~MinoItemizedProperty();

    // Value
    void setValue(qreal value);
    void setValueFromMidi(quint8 value);

    // Step
    qreal step();

    // Items
    void addItem(const QString name, const qreal real);
    MinoItemizedPropertyItem* currentItem();
    void setCurrentItem(const QString name);

    // Type: Linear or Items (default)
    void setLinear(bool linear = true);

signals:
    void itemChanged(QString name);

private:
    QList<MinoItemizedPropertyItem*> _items;
    int _currentItemId;
};

#endif // MINOITEMIZEDPROPERTY_H
