#ifndef UIMIDIPROPERTY_H
#define UIMIDIPROPERTY_H

#include <QWidget>

#include "minoproperty.h"

class UiMidiProperty : public QWidget
{
    Q_OBJECT
public:
    explicit UiMidiProperty(MinoProperty *property, QWidget *parent);
    void setMidiLearnMode(bool on = true) { _midiLearnMode = on; }
protected:
    void paintEvent(QPaintEvent *pe);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    bool _midiLearnMode;

    MinoProperty *_property;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

signals:
    
public slots:
    
private slots:
    void togglePropertyToMaster(bool on);
};

#endif // UIMIDIPROPERTY_H
