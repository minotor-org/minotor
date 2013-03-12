#include "uiknob.h"

#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

#include <qmath.h>

#include "minoproperty.h"

UiKnob::UiKnob(MinoProperty *property, QWidget *parent) :
    QWidget(parent),
    _minValue(0),
    _maxValue(360),
    _value(0),
    _property(property)
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setMaximumHeight(32);
    this->setMaximumWidth(32);

    /*
    if(_property->step() != 0.0)
    {
        setNotchesVisible(true);
        const qreal step = (qreal)127*_property->step();
        setSingleStep(qRound(step));
        //qDebug() << "UiDial should display notches (" << singleStep() << "~=" << step << ")";
    }
    */

    connect(_property, SIGNAL(valueChanged(qreal)), this, SLOT(setValueFromProperty(qreal)));
    this->setMinimum(0);
    this->setMaximum(127);
    this->setValueFromProperty(_property->value());
}

void UiKnob::paintEvent(QPaintEvent *pe)
{
    QRect eventRect = pe->rect();
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
    square.adjust(2.5,2.5,-2.5,-2.5);

    square.moveCenter(eventRect.center());
    painter.setRenderHints(QPainter::RenderHint(QPainter::Antialiasing | QPainter::SmoothPixmapTransform || QPainter::HighQualityAntialiasing));

    QPen pen;
    QColor color(_indicatorColor);
    if (!color.isValid()) { color.setRgb(255,255,255); }
    pen.setWidth(2);
    pen.setColor(color);
    painter.setPen(pen);
    qreal normF = factor();
    qreal invF = 1.0-normF;
    painter.drawArc(square, 220*16, (-260*16)*normF);

    color = _indicatorBackgroundColor;
    if (!color.isValid()) { color.setRgb(0,0,0); }
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawArc(square, (220*16)+((-260*16)*normF), (-260*16)*invF);

    color = _buttonBorderColor;
    if (!color.isValid()) { color.setRgb(0,0,0); }
    pen.setWidth(1);
    pen.setColor(color);
    painter.setPen(pen);
    color = _buttonColor;
    if (!color.isValid()) { color.setRgb(255,255,255); }
    painter.setBrush(QBrush(color));
    const qreal r = ((qreal)square.width()/2.0)-3.5;
    qDebug() << "rayon" << r;
    painter.drawEllipse(square.center(),r,r);

    color = _needleColor;
    if (!color.isValid()) { color.setRgb(255,255,255); }
    pen.setColor(color);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawArc(square.adjusted(7,7,-7,-7),(220*16)+((-260*16)*normF)-4, 8 );

    qDebug() << "UiKnob::paintEvent()" << _value;
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
    _property->setValue(factor());
}

void UiKnob::setValueFromProperty(qreal value)
{
    _value = (qreal)(value*(_maxValue-_minValue))+_minValue;
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


/*!
  \reimp
*/

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


/*!
  \reimp
*/

void UiKnob::mouseMoveEvent(QMouseEvent * e)
{
    if (!(e->buttons() & Qt::LeftButton)) {
        e->ignore();
        return;
    }
    e->accept();
/*
 *    d->doNotEmit = true;
    setSliderPosition(d->valueFromPoint(e->pos()));
    d->doNotEmit = false;
    */
    qDebug() << "UiKnob::mouseMoveEvent()";
    _setValue(valueFromPoint(e->pos()));
    update();
}

QSize UiKnob::minimumSizeHint() const
{
    return QSize(30, 30);
}
