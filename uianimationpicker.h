#ifndef UIANIMATIONPICKER_H
#define UIANIMATIONPICKER_H

#include <QWidget>
#include <QMouseEvent>

#include "minoanimationfactory.h"

class UiAnimationPicker : public QWidget
{
    Q_OBJECT
public:
    explicit UiAnimationPicker(MinoAnimationFactory *factory, QWidget *parent);
    
protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    
public slots:
    
};

#endif // UIANIMATIONPICKER_H
