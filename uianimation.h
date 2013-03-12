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
protected:
    QLabel *_tAnimation;

private:
    QWidget *_wProperties;
    MinoAnimation *_animation;
signals:
    void animationMoved(int programId, int groupId);

public slots:

protected slots:
    void mousePressEvent(QMouseEvent *event);

    void changeAnimationGroup(int programId, int groupId);
};

#endif // UIANIMATION_H
