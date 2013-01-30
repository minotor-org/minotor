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
class Minotor;

class MinoAnimationFactory
{
public:
    template<typename T>
    static void registerClass()
    {
        descriptions().append( T::getDescription() );
        constructors().insert( T::staticMetaObject.className(), &constructorHelper<T> );
    }

    static MinoAnimation* createObject( const QByteArray& className, Minotor* minotor )
    {
        MinoAnimationConstructor constructor = constructors().value( className );
        if ( constructor == NULL )
            return NULL;
        return (*constructor)( minotor );
    }

    static QList<MinoAnimationDescription> availableAnimations()
    {
        return descriptions();
    }

private:
    typedef MinoAnimation* (*MinoAnimationConstructor)( Minotor* minotor );

    template<typename T>
    static MinoAnimation* constructorHelper( Minotor* minotor )
    {
        return new T( minotor );
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
