#ifndef MINOANIMATIONFACTORY_H
#define MINOANIMATIONFACTORY_H

#include <QObject>
#include <QHash>
#include <QString>

#include "minoanimation.h"

/*
 * This code have been greatly inspired from
 *   http://www.mimec.org/node/350
 */
class MinoAnimationGroup;

class MinoAnimationFactory
{
public:
    template<typename T>
    static void registerClass()
    {
        descriptions().append( T::getDescription() );
        constructors().insert( T::staticMetaObject.className(), &constructorHelper<T> );
    }

    static MinoAnimation* createObject( const QByteArray& className, MinoAnimationGroup* group )
    {
        MinoAnimationConstructor constructor = constructors().value( className );
        if ( constructor == NULL )
            return NULL;
        return (*constructor)( group );
    }

    static QList<MinoAnimationDescription> availableAnimations()
    {
        return descriptions();
    }

private:
    typedef MinoAnimation* (*MinoAnimationConstructor)( MinoAnimationGroup* group );

    template<typename T>
    static MinoAnimation* constructorHelper( MinoAnimationGroup* group )
    {
        return new T( group );
    }

    static QHash<QByteArray, MinoAnimationConstructor>& constructors()
    {
        static QHash<QByteArray, MinoAnimationConstructor> instance;
        return instance;
    }

    static QList<MinoAnimationDescription>& descriptions()
    {
        static QList<MinoAnimationDescription> desc;
        return desc;
    }
};

#endif // MINOANIMATIONFACTORY_H
