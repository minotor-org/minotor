#include "minaballs.h"

#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QGraphicsEllipseItem>

MinaBalls::MinaBalls(MinoAnimationGroup *group) :
    MinoAnimation(group)
{
    _beatAnimatedProperty.setStartValue(QVariant(0.0));
    _beatAnimatedProperty.setEndValue(QVariant(1.0));
    //_beatAnimatedProperty.setEasingCurve(QEasingCurve::InOutBounce);

    _borders.append(QLineF(_boundingRect.topLeft(),_boundingRect.topRight()));
    _borders.append(QLineF(_boundingRect.bottomLeft(),_boundingRect.bottomRight()));
    _borders.append(QLineF(_boundingRect.topLeft(),_boundingRect.bottomLeft()));
    _borders.append(QLineF(_boundingRect.topRight(),_boundingRect.bottomRight()));

    // HACK bounding rect center is not really at screen center without that...
    QPointF center = _boundingRect.adjusted(0,0,1,1).center();

    qreal width = 18.0;
    QRectF ball(0,0,width,width);
    QRadialGradient grad(ball.adjusted(0,0,1,1).center(),ball.width());
    grad.setColorAt(0.4, Qt::transparent);
    grad.setColorAt(0.0, Qt::white);

    QGraphicsEllipseItem *item = _scene->addEllipse(ball, QPen(Qt::NoPen), QBrush(grad));
    _itemGroup.addToGroup(item);
    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect(this);
    blur->setBlurRadius(1.1);
    _itemGroup.setGraphicsEffect(blur);

    MinoAnimatedBall maItem = MinoAnimatedBall(
                QLineF(2.0,0.0,20.0,15.0), // path (p1=source, p2=destination)
                _borders.at(1), // destination
                0, 0, item);
    _animatedItems.append(maItem);
}

void MinaBalls::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;
    computeAnimaBeatProperty(gppqn);

     QColor color = _color->color();

    const unsigned int duration = _beatFactor->loopSizeInPpqn();

    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        MinoAnimatedBall *item = const_cast<MinoAnimatedBall*>(&_animatedItems.at(i));
        QGraphicsEllipseItem *ball = dynamic_cast<QGraphicsEllipseItem*>(item->graphicsItem());
        QRadialGradient grad(ball->boundingRect().adjusted(0,0,1,1).center(),ball->boundingRect().width());
        grad.setColorAt(0.4, Qt::transparent);
        grad.setColorAt(0.0, color);
        ball->setBrush(grad);
        if (uppqn > (item->startUppqn()+item->duration()))
        {
            item->_duration = duration;
            item->_startUppqn = uppqn;
            qreal angle = item->_path.angleTo(item->_destination);

            QLineF newPath = QLineF(item->_path.p2(),item->_path.p1());
            newPath.setAngle(angle+item->_destination.angle());
            newPath.setLength(200.0);

            QPointF intersectPoint;
            bool updated = false;
            QLineF destination;
            foreach(QLineF line, _borders)
            {
                if (item->_destination != line)
                {
                    if (newPath.intersect(line, const_cast<QPointF*>(&intersectPoint)) == QLineF::BoundedIntersection)
                    {
                        if(!updated)
                        {
                            newPath.setP2(intersectPoint);
                            destination = line;
                            updated = true;
                        }
                        else
                        {
                            QLineF realPath = newPath;
                            realPath.setP2(intersectPoint);
                            if(realPath.length() < newPath.length())
                            {
                                newPath = realPath;
                                destination = line;
                            }
                        }
                    }
                }
            }

            if (destination.x1() == destination.x2())
            {
                qreal p2y = qMax(qMin(newPath.p2().y(),10.0),6.0);
                newPath.setP2(QPoint(newPath.x2(),p2y));
            }
            else
            {
                qreal p2x = qMax(qMin(newPath.p2().x(),18.0),6.0);
                newPath.setP2(QPoint(p2x, newPath.y2()));
            }
            item->_path = newPath;
            item->_destination = destination;
        }

        const qreal durationFactor = (qreal)(uppqn - item->_startUppqn) / item->_duration;
        _beatAnimatedProperty.setCurrentTime(qreal(_beatAnimatedProperty.duration()) * durationFactor);
        QPointF pos = item->_path.pointAt(_beatAnimatedProperty.currentValue().toReal());
        item->_graphicsItem->setPos(pos-item->_graphicsItem->boundingRect().center());
   }
}
