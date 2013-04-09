#include "minotextproperty.h"

MinoTextProperty::MinoTextProperty(QObject *parent) :
    MinoProperty(parent)
{
    setObjectName("text");
    setLabel("Text");
}

void MinoTextProperty::setTextValue(QString value)
 {
    if(_textValue != value)
    {
        _textValue = value;
        emit textValueChanged(value);
    }
 }
