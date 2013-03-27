#ifndef UIMIDIPROPERTY_H
#define UIMIDIPROPERTY_H

#include <QWidget>

#include "minomidicontrolableproperty.h"

class UiMidiProperty : public QWidget
{
    Q_OBJECT
public:
    explicit UiMidiProperty(MinoMidiControlableProperty *property, QWidget *parent, bool editorMode = false);
    void setMidiLearnMode(bool on = true);

protected:
    void paintEvent(QPaintEvent *pe);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    bool _midiLearnMode;
    bool _midiControlled;

    MinoMidiControlableProperty *_property;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:
    
public slots:
    
private slots:
    void togglePropertyToMaster(bool on);
};

#endif // UIMIDIPROPERTY_H
