#ifndef UIANIMATIONPROPERTY_H
#define UIANIMATIONPROPERTY_H

#include <QWidget>

#include "minoanimationproperty.h"
#include "uidial.h"


class UiAnimationProperty : public QWidget
{
    Q_OBJECT
public:
    explicit UiAnimationProperty(MinoAnimationProperty *property, QWidget *parent);
    void setMidiLearnMode(bool on = true) { _midiLearnMode = on; }

protected:
    void paintEvent(QPaintEvent *pe);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    bool _midiLearnMode;

    UiDial *_dial;

signals:
    
public slots:
    
};

#endif // UIANIMATIONPROPERTY_H
