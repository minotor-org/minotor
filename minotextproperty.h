#ifndef MINOTEXTPROPERTY_H
#define MINOTEXTPROPERTY_H

#include <QString>

#include "minoproperty.h"

class MinoTextProperty : public MinoProperty
{
    Q_OBJECT
public:
    explicit MinoTextProperty();

    QString textValue() { return _textValue; }


signals:
    void textValueChanged(QString value);
public slots:
    void setTextValue(QString value);
protected:
    QString _textValue;

};
#endif // MINOTEXTPROPERTY_H
