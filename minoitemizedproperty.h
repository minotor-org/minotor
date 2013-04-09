#ifndef MINOITEMIZEDPROPERTY_H
#define MINOITEMIZEDPROPERTY_H

#include "minoproperty.h"
#include "midicontrollablelist.h"

class MinoItemizedProperty : public MinoProperty
{
    Q_OBJECT
public:
    explicit MinoItemizedProperty(QObject *parent);

    void setLabel(const QString &label);

    // Items
    void addItem(const QString name, const qreal real);
    MidiControllableListItem* currentItem();
    void setCurrentItem(const QString name);
    void setCurrentItemIndex(int index);
    void setPreferred(bool on = true) { _mcl->setPreferred(on); }

    // Type: Linear or Items (default)
    void setLinear(bool linear = true);

signals:
    void itemChanged(QString name);
    void itemIdChanged(int id);

private:
    MidiControllableList *_mcl;
};

#endif // MINOITEMIZEDPROPERTY_H
