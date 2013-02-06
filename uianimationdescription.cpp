#include "uianimationdescription.h"

UiAnimationDescription::UiAnimationDescription(const MinoAnimationDescription animation, QWidget *parent) :
    QLabel(parent),
    _animation(animation)
{
    this->setText(_animation.name());
    this->setToolTip(_animation.tooltip());
    if(!_animation.pixmap().isNull())
        this->setPixmap(_animation.pixmap());
    this->setMinimumSize(96, 72);
    this->setMaximumSize(96, 72);
    this->setAlignment(Qt::AlignCenter);
}
