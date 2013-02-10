#include "uianimationdescription.h"

UiAnimationDescription::UiAnimationDescription(const MinoAnimationDescription animation, QWidget *parent) :
    QLabel(parent),
    _animation(animation)
{
    this->setText(_animation.name());
    this->setToolTip(_animation.tooltip());
    if(!_animation.pixmap().isNull())
        this->setPixmap(_animation.pixmap().scaled(72,48,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    this->setMinimumSize(72, 48);
    this->setMaximumSize(72, 48);
    this->setAlignment(Qt::AlignCenter);
}
