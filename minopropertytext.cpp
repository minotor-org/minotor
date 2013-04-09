#include "minopropertytext.h"

MinoPropertyText::MinoPropertyText(QObject *parent) :
    MinoProperty(parent)
{
    setObjectName("text");
    setLabel("Text");
}

void MinoPropertyText::setText(const QString& text)
 {
    if(_text != text)
    {
        _text = text;
        emit textChanged(text);
    }
 }
