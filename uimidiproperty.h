#ifndef UIMIDIPROPERTY_H
#define UIMIDIPROPERTY_H

#include <QWidget>

#include "uidial.h"
#include "minoproperty.h"

class UiMidiProperty : public QWidget
{
    Q_OBJECT
public:
    explicit UiMidiProperty(MinoProperty *property, QWidget *parent);
    void setMidiLearnMode(bool on = true) { _midiLearnMode = on; }
    UiDial *dial() { return _dial; }
protected:
    void paintEvent(QPaintEvent *pe);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    bool _midiLearnMode;

    UiDial *_dial;
    MinoProperty *_property;


signals:
    
public slots:
    
};

#endif // UIMIDIPROPERTY_H
