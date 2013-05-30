#include "minaballs.h"

#include <QDebug>
#include <QGraphicsBlurEffect>
#include <QGraphicsEllipseItem>

MinaBalls::MinaBalls(QObject *object) :
    MinoAnimation(object)
{
    _borders.append(QLineF(_boundingRect.topLeft(),_boundingRect.topRight()));
    _borders.append(QLineF(_boundingRect.bottomLeft(),_boundingRect.bottomRight()));
    _borders.append(QLineF(_boundingRect.topLeft(),_boundingRect.bottomLeft()));
    _borders.append(QLineF(_boundingRect.topRight(),_boundingRect.bottomRight()));

    _ballSize = new MinoPropertyReal(this);
    _ballSize->setObjectName("size");
    _ballSize->setLabel("Size");
    _ballSize->setValue(18.0/24.0);

    QRectF ball(0,0,1,1);

    QGraphicsEllipseItem *item = _scene->addEllipse(ball, QPen(Qt::NoPen), QBrush(Qt::NoBrush));
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
    (void)gppqn;
    (void)ppqn;
    (void)qn;

    QColor color = _color->color();

    const unsigned int duration = _beatFactor->loopSizeInPpqn();
    const qreal sideMax = qMax(_boundingRect.height(), _boundingRect.width());
    const qreal sideSize = _ballSize->value()*sideMax;

    QRadialGradient grad;
    grad.setColorAt(0.0,color);
    QColor col(color);

    col.setAlphaF(0.46);
    grad.setColorAt(0.11,col);

    col.setAlphaF(0.21);
    grad.setColorAt(0.22,col);

    col.setAlphaF(0.0983118);
    grad.setColorAt(0.333333,col);

    col.setAlphaF(0.0449751);
    grad.setColorAt(0.444444,col);

    grad.setColorAt(1.0,Qt::transparent);

    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        MinoAnimatedBall *item = const_cast<MinoAnimatedBall*>(&_animatedItems.at(i));
        QGraphicsEllipseItem *ball = dynamic_cast<QGraphicsEllipseItem*>(item->graphicsItem());
        QRectF rect(0.0,0.0,sideSize,sideSize);
        rect.moveCenter(ball->rect().center());
        ball->setRect(rect);

        grad.setCenter(ball->rect().adjusted(0,0,1,1).center());
        grad.setRadius(sideSize);
        grad.setFocalPoint(ball->rect().center());

        ball->setBrush(grad);
        if ((uppqn > (item->startUppqn()+item->duration())))
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
            foreach(const QLineF& line, _borders)
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
                qreal p2y = qMax(qMin(newPath.p2().y(),qreal(10.0)),qreal(6.0));
                newPath.setP2(QPoint(newPath.x2(),p2y));
            }
            else
            {
                qreal p2x = qMax(qMin(newPath.p2().x(),qreal(18.0)),qreal(6.0));
                newPath.setP2(QPoint(p2x, newPath.y2()));
            }
            item->_path = newPath;
            item->_destination = destination;
        }

        const qreal progress = item->progressForUppqn(uppqn);
        QPointF pos = item->_path.pointAt(progress);
        item->_graphicsItem->setPos(pos-item->_graphicsItem->boundingRect().center());
    }
}
