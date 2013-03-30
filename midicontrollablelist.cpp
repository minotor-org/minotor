#include "midicontrollablelist.h"

MidiControllableList::MidiControllableList(QObject *parent) :
    MidiControllableParameter(parent),
    _currentItemId(-1),
    _type(MidiControllableList::Items)
{
}

void MidiControllableList::addItem(const QString name, const qreal real)
{
    _items.append(new MidiControllableListItem(name, real));
}

MidiControllableListItem* MidiControllableList::currentItem()
{
    if(_currentItemId > -1)
    {
        return _items.at(_currentItemId);
        //qDebug() << "value" << (qreal)value << "step" << step << "factor" << tempoList.at(step);
    }
    return NULL;
}

void MidiControllableList::setValueFromMidi(quint8 value)
{
    if(_items.count())
    {
        int id = (qreal)(_items.count() * (qreal)value/128.0);
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
    MidiControllableParameter::setValueFromMidi(value);
}

void MidiControllableList::setCurrentItem(const QString name)
{
    for(int i=0; i<_items.count(); i++)
    {
        if (_items.at(i)->name() == name)
        {
            MidiControllableParameter::setValueFromMidi(((qreal)i/(qreal)_items.count())*127.0);
            if(i != _currentItemId) {
                emit itemChanged(name);
                _currentItemId = i;
            }
            break;
        }
    }
}

void MidiControllableList::setLinear(bool linear)
{
     if(linear)
         setType(MidiControllableList::Steps);
     else
         setType(MidiControllableList::Items);
}
