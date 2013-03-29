#ifndef UIANIMATIONPROPERTY_H
#define UIANIMATIONPROPERTY_H

#include <QWidget>

#include "minoproperty.h"

class UiAnimationProperty : public QWidget
{
    Q_OBJECT
public:
    explicit UiAnimationProperty(MinoProperty *property, QWidget *parent, bool editorMode = false);

protected:

signals:
    
public slots:

};

#endif // UIANIMATIONPROPERTY_H
