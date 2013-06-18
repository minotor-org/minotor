/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        constructors().insert( T::staticMetaObject.className(), &constructorHelper<T> );
    }

    template<typename T>
    static void registerAnimationClass()
    {
        animationModels().append( T::getDescription() );
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
