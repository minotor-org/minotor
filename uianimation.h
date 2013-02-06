#ifndef UIANIMATION_H
#define UIANIMATION_H

#include <QGroupBox>

#include <QLabel>

#include "minoanimation.h"

class UiAnimation : public QGroupBox
{
    Q_OBJECT
public:
    explicit UiAnimation(MinoAnimation *animation, QWidget *parent);

protected:
    QLabel *_tAnimation;

private:

signals:
    void customContextMenuRequested(const QPoint &pos);

public slots:
    void enable(const bool on);
};

#endif // UIANIMATION_H
