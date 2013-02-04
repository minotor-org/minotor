#ifndef MINOITEMIZEDPROPERTY_H
#define MINOITEMIZEDPROPERTY_H

#include "minoproperty.h"

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

class MinoItemizedProperty : public MinoProperty
{
    Q_OBJECT
public:
    explicit MinoItemizedProperty();
    ~MinoItemizedProperty();

    // value
    void setValue(qreal value);

    // step
    qreal step();

    // items
    void addItem(const QString name, const qreal real);
    MinoItemizedPropertyItem* currentItem();
    void setCurrentItem(const QString name);

signals:
    void itemChanged(QString name);

private:
    QList<MinoItemizedPropertyItem*> _items;
    int _currentItemId;
};

#endif // MINOITEMIZEDPROPERTY_H
