#ifndef UIANIMATION_H
#define UIANIMATION_H

#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>

#include "minoanimation.h"

class UiAnimation : public QGroupBox
{
    Q_OBJECT
public:
    explicit UiAnimation(MinoAnimation *animation, QWidget *parent);
    void setExpanded(bool expanded);
    MinoAnimation *animation() { return _animation; }

    template<typename T> T findParent();

protected:
    QLabel *_tAnimation;

private:
    QWidget *_wProperties;
    MinoAnimation *_animation;

public slots:

protected slots:
    void mousePressEvent(QMouseEvent *event);

    void takeAShot();

};

#endif // UIANIMATION_H