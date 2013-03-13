#ifndef UIANIMATIONPROPERTY_H
#define UIANIMATIONPROPERTY_H

#include "uimidiproperty.h"

class UiAnimationProperty : public UiMidiProperty
{
    Q_OBJECT
public:
    explicit UiAnimationProperty(MinoProperty *property, QWidget *parent);

protected:

signals:
    
public slots:

private slots:
    void togglePropertyToMaster(bool on);
};

#endif // UIANIMATIONPROPERTY_H
