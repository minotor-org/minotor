#include "minotextproperty.h"

MinoTextProperty::MinoTextProperty(QObject *parent) :
    MinoProperty(parent)
{

}
void MinoTextProperty::setTextValue(QString value)
 {
    if(_textValue != value)
    {
        _textValue = value;
        emit textValueChanged(value);
    }
 }
