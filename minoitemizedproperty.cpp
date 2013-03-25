#include "minoitemizedproperty.h"

#include <QDebug>

MinoItemizedProperty::MinoItemizedProperty():
    MinoMidiControlableProperty(),
    _currentItemId(-1)
{
    setType(MinoMidiControlableProperty::Items);
}

MinoItemizedProperty::~MinoItemizedProperty()
{
    foreach(MinoItemizedPropertyItem* item, _items)
        delete item;
}

void MinoItemizedProperty::addItem(const QString name, const qreal real)
{
    _items.append(new MinoItemizedPropertyItem(name, real));
}

MinoItemizedPropertyItem* MinoItemizedProperty::currentItem()
{
    if(_currentItemId > -1)
    {
        return _items.at(_currentItemId);
        //qDebug() << "value" << (qreal)value << "step" << step << "factor" << tempoList.at(step);
    }
    return NULL;
}

void MinoItemizedProperty::setValue(qreal value)
{
    if(_items.count())
    {
        int id = (qreal)(_items.count() * value);
        if (id != _currentItemId)
        {
            _currentItemId = id;
            emit itemChanged(_items.at(_currentItemId)->name());
        }
    }
    else
    {
        _currentItemId = -1;
    }
}

void MinoItemizedProperty::setValueFromMidi(quint8 value)
{
    setValue((qreal)value/128);
}

qreal MinoItemizedProperty::step()
{
    if(_items.count())
    {
        return (qreal) 1.0 / _items.count();
    }
    return 0.0;
}

void MinoItemizedProperty::setCurrentItem(const QString name)
{
    for(int i=0; i<_items.count(); i++)
    {
        if (_items.at(i)->name() == name)
        {
            _midiValue = ((qreal)i/(qreal)_items.count())*128.0;
            if(i != _currentItemId) {
                emit itemChanged(name);
                _currentItemId = i;
            }
            break;
        }
    }
}

void MinoItemizedProperty::setLinear(bool linear)
{
     if(linear)
         setType(MinoMidiControlableProperty::Steps);
     else
         setType(MinoMidiControlableProperty::Items);
}
