#include "minarotatingbars.h"

#include <QDebug>
#include <QGraphicsEffect>

#include <math.h>

MinaRotatingBars::MinaRotatingBars(QObject *object) :
    MinoAnimation(object)
{
    _ecrAngle.setStartValue(1.0);
    _ecrAngle.setEndValue(0.0);
    _ecrAngle.setEasingCurve(QEasingCurve::OutBounce);

    _width = new MinoPropertyReal(this);
    _width->setObjectName("width");
    _width->setLabel("Width");
    _width->setValue(0.1);

    _length = new MinoPropertyReal(this);
    _length->setObjectName("length");
    _length->setLabel("Length");
    _length->setValue(0.6);

    _generatorCurve = new MinoPropertyEasingCurve(this, true);

    QColor color;
    color.setHsvF(0.4, 1.0, 1.0);

    _maxPixels = int(sqrt(pow(_boundingRect.bottomRight().x(), 2) + pow(_boundingRect.bottomRight().y(), 2)))+1;

    _drawingZone = QRectF(0,0,_maxPixels,_maxPixels);
    _drawingZone.moveCenter(_boundingRect.center());

    //Left rect
    _items[0] = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    _itemGroup.addToGroup(_items[0]);

    //Top rect
    _items[1] = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    _itemGroup.addToGroup(_items[1]);

    //Right rect
    _items[2] = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    _itemGroup.addToGroup(_items[2]);

    //Bottom rect
    _items[3] = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    _itemGroup.addToGroup(_items[3]);

    _itemGroup.setVisible(false);
}

MinaRotatingBars::~MinaRotatingBars()
{
    //delete _itemGroup;
}

void MinaRotatingBars::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    qreal centerX = _drawingZone.adjusted(0,0,1,1).center().x();
    qreal centerY = _drawingZone.adjusted(0,0,1,1).center().y();

    QColor color = _color->color();

    qreal rectWidth = (_maxPixels/2)*_width->value();
    qreal rectLength = (_maxPixels/2)*_length->value();

    //Left rect
    QRectF rect0Coords = QRectF(_drawingZone.x(),_drawingZone.y()+((_drawingZone.height()/2.0)-(rectWidth/2.0)),rectLength,rectWidth);
    _items[0]->setBrush(QBrush(color));
    _items[0]->setRect(rect0Coords);

    //Top rect
    QRectF rect1Coords = QRectF(_drawingZone.x()+((_drawingZone.width()/2)-(rectWidth/2.0)),_drawingZone.y(), rectWidth, rectLength);
    _items[1]->setBrush(QBrush(color));
    _items[1]->setRect(rect1Coords);

    //Right rect
    QRectF rect2Coords = QRectF((_drawingZone.x()+_drawingZone.width())-rectLength,_drawingZone.y()+((_drawingZone.height()/2.0)-(rectWidth/2.0)),rectLength,rectWidth);
    _items[2]->setBrush(QBrush(color));
    _items[2]->setRect(rect2Coords);

    //Bottom rect
    QRectF rect3Coords = QRectF(_drawingZone.x()+((_drawingZone.width()/2)-(rectWidth/2.0)),(_drawingZone.y()+_drawingZone.height())-rectLength, rectWidth, rectLength);
    _items[3]->setBrush(QBrush(color));
    _items[3]->setRect(rect3Coords);

    _ecrAngle.setEasingCurve(_generatorCurve->easingCurveType());

    _itemGroup.setTransform(QTransform().translate(centerX, centerY).rotate((_ecrAngle.valueForProgress(_beatFactor->progressForGppqn(gppqn))*90.0)).translate(-centerX, -centerY));
}
