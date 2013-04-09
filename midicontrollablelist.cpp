#include "midicontrollablelist.h"

MidiControllableList::MidiControllableList(QObject *parent) :
    MidiControllableParameter(parent),
    _type(MidiControllableList::Items),
    _currentItemId(-1)
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
            emit itemIdChanged(_currentItemId);
            emit itemChanged(_items.at(_currentItemId)->name());
            emit itemValueChanged(_items.at(_currentItemId)->real());
        }
    }
    else
    {
        _currentItemId = -1;
    }
    MidiControllableParameter::setValueFromMidi(value);
}

void MidiControllableList::setCurrentItemIndex(int index)
{
    if(_currentItemId != index)
    {
        _currentItemId = index;
        emit itemIdChanged(_currentItemId);
        emit itemChanged(_items.at(_currentItemId)->name());
        emit itemValueChanged(_items.at(_currentItemId)->real());
    }
}

void MidiControllableList::setCurrentItem(const QString& name)
{
    if((_currentItemId==-1) || _items.at(_currentItemId)->name() != name)
    {
        for(int i=0; i<_items.count(); i++)
        {
            if (_items.at(i)->name() == name)
            {
                MidiControllableParameter::setValueFromMidi(((qreal)i/(qreal)_items.count())*127.0);
                if(i != _currentItemId) {
                    _currentItemId = i;
                    emit itemIdChanged(_currentItemId);
                    emit itemChanged(_items.at(_currentItemId)->name());
                    emit itemValueChanged(_items.at(_currentItemId)->real());
                }
                break;
            }
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
