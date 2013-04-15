#include "uieasingcurve.h"

#include <QPainter>
#include <QDebug>

UiEasingCurve::UiEasingCurve(QWidget *parent) :
    QWidget(parent),
    _axesColor(Qt::black),
    _gridColor(Qt::gray),
    _curveColor(Qt::red)
{
}

void UiEasingCurve::paintEvent(QPaintEvent *pe)
{
    (void)pe;
    QRectF square = this->rect();
    if (square.height() > square.width())
    {
        square.setHeight(square.width());
    }
    else
    {
        square.setWidth(square.height());
    }
    square.moveCenter(this->rect().center());

    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint(QPainter::Antialiasing | QPainter::SmoothPixmapTransform || QPainter::HighQualityAntialiasing));

    // Grid
    QPen gridPen(_gridColor);
    gridPen.setStyle(Qt::DotLine);
    gridPen.setWidth(1);
    painter.setPen(gridPen);

    QPainterPath grid;
    for (qreal p=0.25;p<1.0;p+=0.25)
    {
        const QPointF bottom = square.bottomLeft()+QPoint(p*square.width(),0.0);
        const QPointF top = square.topLeft()+QPoint(p*square.width(),0.0);
        const QPointF left = square.topLeft()+QPoint(0.0,p*square.height());
        const QPointF right = square.topRight()+QPoint(0.0,p*square.height());
        grid.moveTo(bottom); grid.lineTo(top);
        grid.moveTo(left); grid.lineTo(right);
    }
    painter.drawPath(grid);

    // Axes
    QPen axesPen(_axesColor);
    axesPen.setWidth(2);
    painter.setPen(axesPen);

    QPainterPath axes;
    axes.moveTo(square.topLeft());
    axes.lineTo(square.bottomLeft());
    axes.lineTo(square.bottomRight());
    painter.drawPath(axes);

    // Curve
    QPen curvePen(_curveColor);
    curvePen.setWidth(1);
    painter.setPen(curvePen);

    QPainterPath curve;
    bool init = false;
    for (qreal p=0.0;p<=1.0;p+=0.05)
    {
        const QPointF point = square.bottomLeft()+QPointF(p*square.width(),-_ec.valueForProgress(p)*square.height());
        if(!init)
        {
            curve.moveTo(point);
            init = true;
        }
        else
        {
            curve.lineTo(point);
        }
    }
    painter.drawPath(curve);
}

void UiEasingCurve::setEasingCurve(const QEasingCurve &ec)
{
    _ec = ec;
    update();
}
