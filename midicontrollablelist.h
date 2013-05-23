#ifndef MIDICONTROLLABLELIST_H
#define MIDICONTROLLABLELIST_H

#include "midicontrollableparameter.h"

class MidiControllableListItem
{
public:
    explicit MidiControllableListItem(const QString name, const qreal real):
        _name(name),
        _real(real)
    {}

    QString name() const { return _name; }
    qreal real() const { return _real; }

private:
    QString _name;
    qreal _real;
};

class MidiControllableList : public MidiControllableParameter
{
    Q_OBJECT
public:
    explicit MidiControllableList(QObject *parent = 0);

    // Type
    enum Type { Items, Steps };
    MidiControllableList::Type type() { return _type; }
    void setType(MidiControllableList::Type type) { _type = type; }

    unsigned int itemsCount() { return _items.count(); }

    // Items
    void addItem(const QString name, const qreal real);
    MidiControllableListItem* currentItem();
    void setCurrentItem(const QString &name);
    void setCurrentItemIndex(int index);
    int currentItemIndex() { return _currentItemId; }

    // Type: Linear or Items (default)
    void setLinear(bool linear = true);

private:
    MidiControllableList::Type _type;
    QList<MidiControllableListItem*> _items;
    int _currentItemId;

signals:
    void itemChanged(QString name);
    void itemIdChanged(int id);
    void itemValueChanged(qreal value);

public slots:
    void setValueFromMidi(quint8 value);
};

#endif // MIDICONTROLLABLELIST_H
