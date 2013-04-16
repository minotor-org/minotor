#ifndef MINOPERSISTENTOBJECTFACTORY_H
#define MINOPERSISTENTOBJECTFACTORY_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QDebug>

#include "minopersistentobject.h"
#include "minoanimation.h"

/*
 * This code have been greatly inspired from
 *   http://www.mimec.org/node/350
 */
class MinoPersistentObjectFactory
{
public:
    template<typename T>
    static void registerClass()
    {
        const QMetaObject *mo = T::staticMetaObject.superClass();
        if(mo)
        {
            const QString superclass = mo->className();
            if(superclass == QString("MinoAnimation"))
            {
                animationModels().append( T::getDescription() );
            }
        }
        constructors().insert( T::staticMetaObject.className(), &constructorHelper<T> );
    }

    static MinoPersistentObject* createObject( const QByteArray& className, QObject* object )
    {
        MinoPersistentObjectConstructor constructor = constructors().value( className );
        if ( constructor == NULL )
            return NULL;
        return (*constructor)( object );
    }

    static QList<MinoAnimationDescription> availableAnimationModels()
    {
        return animationModels();
    }

private:
    typedef MinoPersistentObject* (*MinoPersistentObjectConstructor)( QObject *object );

    template<typename T>
    static MinoPersistentObject* constructorHelper( QObject *object )
    {
        return new T( object );
    }

    static QHash<QByteArray, MinoPersistentObjectConstructor>& constructors()
    {
        static QHash<QByteArray, MinoPersistentObjectConstructor> instance;
        return instance;
    }

    static QList<MinoAnimationDescription>& animationModels()
    {
        static QList<MinoAnimationDescription> _animationModels;
        return _animationModels;
    }
};

#endif // MINOPERSISTENTOBJECTFACTORY_H
