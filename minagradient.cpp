#include "minagradient.h"
#include "minoprogram.h"
#include <QDebug>

MinaGradient::MinaGradient(QObject *object):
    MinoAnimation(object)
{
    _beatAnimatedProperty.setStartValue(QVariant(0.0));
    _beatAnimatedProperty.setEndValue(QVariant(1.0));

    _generatorStyle = new MinoItemizedProperty(this);
    _generatorStyle->setObjectName("style");
    _generatorStyle->setLabel("Style");
    _generatorStyle->addItem("Radial", 0);
    _generatorStyle->addItem("Conical", 1);
    _generatorStyle->setCurrentItem("Radial");

    _generatorCurve = new MinoPropertyEasingCurve(this, true);

    QColor color = _color->color();

    _rectItem = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    _rectItem->setVisible(false);
}

MinaGradient::~MinaGradient()
{
    delete _rectItem;
}

void MinaGradient::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    QColor color = _color->color();

    // HACK bounding rect center is not really at screen center without that...
    QPointF center = _boundingRect.adjusted(0,0,1,1).center();

    QGradient grad;

    switch ((int)_generatorStyle->currentItem()->real())
    {
    case 0 :
    {
        // Water waves style
        grad = QRadialGradient(center,_boundingRect.width());
    }
        break;
    case 1 :
    {
        // Spotlight style
        center.setY(-1);
        grad = QConicalGradient(center,_boundingRect.height());
    }
        break;
    }

    _beatAnimatedProperty.setEasingCurve(_generatorCurve->easingCurveType());

    const unsigned int waves = 6;
	const qreal step = 1.0 / ((qreal) waves *2.0);
    const qreal anipos = _beatAnimatedProperty.currentValue().toReal();
    bool toogle = true;
    for (qreal pos = 0.0; pos <= 1.0; pos+=step)
    {
        qreal at = anipos + pos;
        if(at > 1.0) at -= 1.0;

        if(toogle)
        {
            grad.setColorAt(at, Qt::transparent);
        }
        else
        {
            grad.setColorAt(at, color) ;
        }
        toogle = !toogle;
    }

    _rectItem->setBrush(QBrush(grad));

}
