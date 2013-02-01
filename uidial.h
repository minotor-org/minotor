#ifndef UIDIAL_H
#define UIDIAL_H

#include <QDial>

#include "minoanimationproperty.h"

class UiDial : public QDial
{
    Q_OBJECT
public:
    explicit UiDial(MinoAnimationProperty *property, QWidget *parent = 0);
    MinoAnimationProperty *property() { return _property; }

    void setMidiLearnMode(bool on = true) { _midiLearnMode = on; }

    void paintEvent(QPaintEvent *pe);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

protected:
    MinoAnimationProperty *_property;
    bool _midiLearnMode;
signals:
    
public slots:
    void setValueFromProperty(qreal value);
    void setAnimationPropertyValue(int value);
private:
};

#endif // UIDIAL_H
