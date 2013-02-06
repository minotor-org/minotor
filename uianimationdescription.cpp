#include "uianimationdescription.h"

UiAnimationDescription::UiAnimationDescription(const MinoAnimationDescription animation, QWidget *parent) :
    QLabel(parent),
    _animation(animation)
{
    this->setText(_animation.name());
    this->setToolTip(_animation.tooltip());
    if(!_animation.pixmap().isNull())
        this->setPixmap(_animation.pixmap());
    this->setMinimumSize(80, 60);
    this->setMaximumSize(80, 60);
    this->setAlignment(Qt::AlignCenter);
}
