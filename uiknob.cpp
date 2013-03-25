#include "uiknob.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include <qmath.h>

#include "minomidicontrolableproperty.h"

UiKnob::UiKnob(MinoMidiControlableProperty *property, QWidget *parent):
    QWidget(parent),
    _indicatorColor(255,255,255),
    _indicatorBackgroundColor(0,0,0),
    _buttonColor(255,255,255),
    _buttonBorderColor(0,0,0),
    _needleColor(255,255,255),
    _reflectColor(0,0,0),
    _minValue(0),
    _maxValue(360),
    _value(0),
    _property(property)
{
    this->setFocusPolicy(Qt::NoFocus);

    connect(_property, SIGNAL(midiValueChanged(quint8)), this, SLOT(setValueFromMidi(quint8)));
    this->setMinimum(0);
    this->setMaximum(127);
    this->setValueFromMidi(_property->midiValue());
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void UiKnob::paintEvent(QPaintEvent *pe)
{
    (void)pe;

    QRect eventRect = this->rect();
    QPainter painter(this);

    QRectF square = eventRect;
    if (square.height() > square.width())
    {
        square.setHeight(square.width());
    }
    else
    {
        square.setWidth(square.height());
    }
    square.setSize(square.size()*(1.0/ratio()));
    square.adjust(2.5,2.5,-2.5,-2.5);

    QPoint center(eventRect.center());
    center.setY(center.y()*(1.0/ratio()));
    square.moveCenter(center);

    painter.setRenderHints(QPainter::RenderHint(QPainter::Antialiasing | QPainter::SmoothPixmapTransform || QPainter::HighQualityAntialiasing));

    QPen pen;
    QColor color;
    const qreal normF = factor();
    const qreal startAngle = 220.0*16.0;
    const qreal spanAngle = -260.0*16.0;

    switch (_property->type())
    {
    case MinoMidiControlableProperty::Linear:
    {
        color = _indicatorColor;
        pen.setWidth(2);
        pen.setColor(color);
        painter.setPen(pen);
        qreal invF = 1.0-normF;
        painter.drawArc(square, startAngle, spanAngle*normF);

        color = _indicatorBackgroundColor;
        pen.setColor(color);
        painter.setPen(pen);
        painter.drawArc(square, startAngle+(spanAngle*normF), spanAngle*invF);
    }
        break;
    case MinoMidiControlableProperty::Steps:
    {
        color = _indicatorColor;
        QPen indicatorPen(color);
        indicatorPen.setWidth(2);
        painter.setPen(indicatorPen);
        color = _indicatorBackgroundColor;
        QPen indicatorBackgroundPen(color);
        indicatorBackgroundPen.setWidth(2);

        const qreal step = _property->step();
        const qreal outBoundedFactor = qMin(normF, 0.999999);
        int pos = (qreal)(outBoundedFactor/step);
        const int count = qRound(1.0/step);

        for(int i=0; i<count; i++)
        {
            if(i>pos)
            {
                painter.setPen(indicatorBackgroundPen);
            }
            painter.drawArc(square, startAngle+(spanAngle*i*step)-160,spanAngle*step+320);
        }
    }
        break;
    case MinoMidiControlableProperty::Items:
    {
        color = _indicatorColor;
        QPen indicatorPen(color);
        indicatorPen.setWidth(2);
        painter.setPen(indicatorPen);
        color = _indicatorBackgroundColor;
        QPen indicatorBackgroundPen(color);
        indicatorBackgroundPen.setWidth(2);

        const qreal step = _property->step();
        const qreal outBoundedFactor = qMin(normF, 0.999999);
        int pos = (qreal)(outBoundedFactor/step);
        const int count = qRound(1.0/step);

        for(int i=0; i<count; i++)
        {
            if(i != pos)
            {
                painter.setPen(indicatorBackgroundPen);
            }
            else
            {
                painter.setPen(indicatorPen);
            }
            painter.drawArc(square, startAngle+(spanAngle*i*step)-160,spanAngle*step+320);
        }
    }
        break;
    }

    color = _buttonBorderColor;
    pen.setWidth(1);
    pen.setColor(color);
    painter.setPen(pen);
    color = _buttonColor;
    painter.setBrush(QBrush(color));
    const qreal r = ((qreal)square.width()/2.0)-3.5;
    painter.drawEllipse(square.center(),r,r);

    color = _reflectColor;
    pen.setWidth(0.5);
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawArc(square.adjusted(5,5,-5,-5),(60*16), (150*16) );

    color = _needleColor;
    pen.setColor(color);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawArc(square.adjusted(7,7,-7,-7),(220*16)+((-260*16)*normF)-4, 8 );
}

qreal UiKnob::factor() const
{
    return ((_value-_minValue)/_maxValue);
}

#define Q_PI 3.14957
qreal UiKnob::valueFromPoint(const QPoint &p)
{
    const int maxDistance = 100;
    int deltaY = _mouseStartPos.y() - p.y();

    _mouseStartPos.setX(p.x());
    _mouseStartPos.setY(p.y());
    deltaY = qBound(-maxDistance, deltaY, maxDistance);

    qreal deltaForValue = (qreal)((qreal)deltaY/(qreal)maxDistance) * (qreal)(_maxValue-_minValue);
    qreal value = (qreal)_value + deltaForValue;
    value = qBound(_minValue, value, _maxValue);
    qDebug() << "UiKnob::valueFromPoint>"
             << "value" << value
             << "deltaY" << deltaY
             << "deltaForValue" << deltaForValue
             << "_value" << _value;
    return value;
}

void UiKnob::_setValue(qreal value)
{
    _value = value;
    disconnect(_property, SIGNAL(midiValueChanged(quint8)), this, SLOT(setValueFromMidi(quint8)));
    _property->setMidiValue(value);
    connect(_property, SIGNAL(midiValueChanged(quint8)), this, SLOT(setValueFromMidi(quint8)));
}

void UiKnob::setValueFromMidi(quint8 value)
{
    _value = ((qreal)value/127.0*(_maxValue-_minValue))+_minValue;
    update();
}

void UiKnob::mousePressEvent(QMouseEvent *e)
{
    if ((e->button() != Qt::LeftButton)  ||
        (e->buttons() ^ e->button())) {
        e->ignore();
        return;
    }
    e->accept();
    qDebug() << "UiKnob::mousePressEvent()";
    _mouseStartPos = e->pos();
    _setValue(valueFromPoint(e->pos()));
    update();
}

void UiKnob::mouseReleaseEvent(QMouseEvent * e)
{
    if (e->buttons() & (~e->button()) ||
       (e->button() != Qt::LeftButton)) {
        e->ignore();
        return;
    }
    qDebug() << "UiKnob::mouseReleaseEvent()";
    e->accept();
    _setValue(valueFromPoint(e->pos()));
    update();
}

void UiKnob::mouseMoveEvent(QMouseEvent * e)
{
    if (!(e->buttons() & Qt::LeftButton)) {
        e->ignore();
        return;
    }
    e->accept();
    /*
    d->doNotEmit = true;
    setSliderPosition(d->valueFromPoint(e->pos()));
    d->doNotEmit = false;
    */
    qDebug() << "UiKnob::mouseMoveEvent()";
    _setValue(valueFromPoint(e->pos()));
    update();
}

void UiKnob::wheelEvent(QWheelEvent * event)
{
    if (_property->step() != 0.0)
    {
        qreal value = _value;
        value += (_property->step()*((qreal)event->delta()/120.0))*(_maxValue-_minValue);
        value = qBound(_minValue,value,_maxValue);
        _setValue(value);
    }
    else
    {
        qreal value = _value;
        value += (0.1*((qreal)event->delta()/120.0))*(_maxValue-_minValue);
        value = qBound(_minValue,value,_maxValue);
        _setValue(value);
    }
    update();
}

QSize UiKnob::minimumSizeHint() const
{
    return QSize(32, ratio()*32);
}

QSize UiKnob::sizeHint() const
{
    return QSize(60, ratio()*32);
}
