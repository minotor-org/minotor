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
class MinoProgram;

class MinoAnimationFactory
{
public:
    template<typename T>
    static void registerClass()
    {
        descriptions().append( T::getDescription() );
        constructors().insert( T::staticMetaObject.className(), &constructorHelper<T> );
    }

    static MinoAnimation* createObject( const QByteArray& className, MinoProgram* program )
    {
        MinoAnimationConstructor constructor = constructors().value( className );
        if ( constructor == NULL )
            return NULL;
        return (*constructor)( program );
    }

    static QList<MinoAnimationDescription> availableAnimations()
    {
        return descriptions();
    }

private:
    typedef MinoAnimation* (*MinoAnimationConstructor)( MinoProgram* program );

    template<typename T>
    static MinoAnimation* constructorHelper( MinoProgram* program )
    {
        return new T( program );
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
