#ifndef UIANIMATIONDESCRIPTION_H
#define UIANIMATIONDESCRIPTION_H

#include <QLabel>

#include "minoanimation.h"

class UiAnimationDescription : public QLabel
{
    Q_OBJECT
public:
    explicit UiAnimationDescription(const MinoAnimationDescription animation, QWidget *parent);

    MinoAnimationDescription *description() { return &_animation; }
signals:
    
public slots:
    
private:
    MinoAnimationDescription _animation;
};

#endif // UIANIMATIONDESCRIPTION_H
