#include "minoanimationfactory.h"

#include "minoanimationrandompixels.h"
#include "minoanimationexpandingobjects.h"
#include "minoanimationdebug.h"
#include "minoanimationwaveform.h"
#include "minoanimationbarsfromsides.h"

#include <QDebug>

MinoAnimationFactory::MinoAnimationFactory(QObject *parent) :
    QObject(parent)
{
    _animations.insert("MinoAnimationDebug", MinoAnimationDebug::getDescription());
    _animations.insert("MinoAnimationExpandingObjects", MinoAnimationExpandingObjects::getDescription());
    _animations.insert("MinoAnimationBarsFromSides", MinoAnimationBarsFromSides::getDescription());
    _animations.insert("MinoAnimationRandomPixels", MinoAnimationRandomPixels::getDescription());
    _animations.insert("MinoAnimationWaveform", MinoAnimationWaveform::getDescription());
}

QList<MinoAnimationDescription> MinoAnimationFactory::availableAnimations()
{
    return _animations.values();
}

MinoAnimation *MinoAnimationFactory::instantiate(const QString className, MinoChannel *channel)
{
    if(_animations.contains(className))
    {
        if(className == "MinoAnimationBarsFromSides") {
            return new MinoAnimationBarsFromSides(channel);
        } else {
            qDebug() << "Implement me !!!";
        }
    }
    return NULL;
}
