#ifndef UIKNOB_H
#define UIKNOB_H

#include <QWidget>
#include <QPoint>

class MinoMidiControlableProperty;

class UiKnob : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor indicator_color READ indicatorColor WRITE setIndicatorColor DESIGNABLE true)
    Q_PROPERTY(QColor indicator_background_color READ indicatorBackgroundColor WRITE setIndicatorBackgroundColor DESIGNABLE true)
    Q_PROPERTY(QColor button_color READ buttonColor WRITE setButtonColor DESIGNABLE true)
    Q_PROPERTY(QColor button_border_color READ buttonBorderColor WRITE setButtonBorderColor DESIGNABLE true)
    Q_PROPERTY(QColor needle_color READ needleColor WRITE setNeedleColor DESIGNABLE true)
    Q_PROPERTY(QColor reflect_color READ reflectColor WRITE setReflectColor DESIGNABLE true)
public:
    explicit UiKnob(MinoMidiControlableProperty *property, QWidget *parent);
    
    QColor indicatorColor() { return _indicatorColor; }
    void setIndicatorColor(QColor color) { _indicatorColor = color; }

    QColor indicatorBackgroundColor() { return _indicatorBackgroundColor; }
    void setIndicatorBackgroundColor(QColor color) { _indicatorBackgroundColor = color; }

    QColor buttonColor() { return _buttonColor; }
    void setButtonColor(QColor color) { _buttonColor = color; }

    QColor buttonBorderColor() { return _buttonBorderColor; }
    void setButtonBorderColor(QColor color) { _buttonBorderColor = color; }

    QColor needleColor() { return _needleColor; }
    void setNeedleColor(QColor color) { _needleColor = color; }

    QColor reflectColor() { return _reflectColor; }
    void setReflectColor(QColor color) { _reflectColor = color; }

    void setMinimum(qreal minimum) { _minValue = minimum; }
    void setMaximum(qreal maximum) { _maxValue = maximum; }
    void setValue(qreal value) { _value = value; }
    MinoMidiControlableProperty *property() { return _property; }

signals:
    
private slots:
    void setValueFromMidi(quint8 value);

protected:
    qreal valueFromPoint(const QPoint &p);

    void paintEvent(QPaintEvent *pe);

    void mousePressEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void wheelEvent(QWheelEvent * event);

    // notches
    QColor _indicatorColor;
    QColor _indicatorBackgroundColor;
    QColor _buttonColor;
    QColor _buttonBorderColor;
    QColor _needleColor;
    QColor _reflectColor;

    // Value
    qreal _minValue;
    qreal _maxValue;
    qreal _value;

    // Mouse
    QPoint _mouseStartPos;

    // Associated MinoMidiControlableProperty
    MinoMidiControlableProperty *_property;

    qreal factor() const;
private:
    void _setValue(qreal value);
    qreal ratio() const { return 0.78; }
};

#endif // UIKNOB_H
