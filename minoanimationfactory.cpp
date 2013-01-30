#include "minoanimationfactory.h"

#include "minarandompixels.h"
#include "minaexpandingobjects.h"
#include "minadebug.h"
#include "minawaveform.h"
#include "minabarsfromsides.h"

#include <QDebug>

MinoAnimationFactory::MinoAnimationFactory(QObject *parent) :
    QObject(parent)
{
    _animations.insert("MinaDebug", MinaDebug::getDescription());
    _animations.insert("MinaExpandingObjects", MinaExpandingObjects::getDescription());
    _animations.insert("MinaBarsFromSides", MinaBarsFromSides::getDescription());
    _animations.insert("MinaRandomPixels", MinaRandomPixels::getDescription());
    _animations.insert("MinaWaveform", MinaWaveform::getDescription());
}

QList<MinoAnimationDescription> MinoAnimationFactory::availableAnimations()
{
    return _animations.values();
}

MinoAnimation *MinoAnimationFactory::instantiate(const QString className, Minotor *minotor)
{
    if(_animations.contains(className))
    {
        if(className == "MinaBarsFromSides") {
            return new MinaBarsFromSides(minotor);
        } else {
            qDebug() << "Implement me !!!";
        }
    }
    return NULL;
}
