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
protected:
    QLabel *_tAnimation;

private:
    QWidget *_wProperties;
    QWidget *_wEnable;
    QCheckBox *_cbEnable;
signals:
    void customContextMenuRequested(const QPoint &pos);

public slots:
    void enable(const bool on);
};

#endif // UIANIMATION_H
