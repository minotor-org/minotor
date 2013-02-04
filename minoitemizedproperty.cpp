#include "minoitemizedproperty.h"

MinoItemizedProperty::MinoItemizedProperty():
    MinoProperty(),
    _currentItemId(-1)
{
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
    MinoProperty::setValue(value);

    if(_items.count())
    {
        // Note: Reduce _value (factor) 0.01 is needed to not reach _items.count() and keep linear segmentation
        int id = (qreal)(_items.count()) * (_value-0.01);
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
            MinoProperty::setValue((qreal)i/(qreal)_items.count());
            if(i != _currentItemId) {
                emit itemChanged(name);
                _currentItemId = i;
            }
            break;
        }
    }
}
