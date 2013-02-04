#ifndef UIDIAL_H
#define UIDIAL_H

#include <QDial>

#include "minoproperty.h"

class UiDial : public QDial
{
    Q_OBJECT
public:
    explicit UiDial(MinoProperty *property, QWidget *parent = 0);
    MinoProperty *property() { return _property; }

protected:
    MinoProperty *_property;

signals:
    
public slots:
    void setValueFromProperty(qreal value);
    void setAnimationPropertyValue(int value);
private:
};

#endif // UIDIAL_H
