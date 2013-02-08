#include "minotextproperty.h"

MinoTextProperty::MinoTextProperty() :
    MinoProperty()
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
