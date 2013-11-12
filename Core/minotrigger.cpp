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

#include "minotrigger.h"
#include <QDebug>

MinoTrigger::MinoTrigger(QString role, QObject *parent) :
    QObject(parent),
    _on(false),
    _role(role),
    _feedback(false),
    _sender(NULL),
    _signal(NULL)
{
}

void MinoTrigger::setStatus(bool on)
{
    if(on != _on)
    {
        _on = on;
        emit toggled(on);
        if(on)
            emit triggered();
    }
}

void MinoTrigger::setFeedback(bool on)
{
    _feedback = on;
    emit feedback(on);
}

void MinoTrigger::forceFeedbackEmitting()
{
    qDebug() << Q_FUNC_INFO << this << role() << _feedback;
    // Re-send previously saved feedback value
    emit feedback(_feedback);
}

void MinoTrigger::connectFeedback(const QObject *sender, const char *signal)
{
    if(sender && signal)
    {
        _sender = sender;
        _signal = signal;

        connect(sender, signal, this, SLOT(setFeedback(bool)));
//            qDebug() << Q_FUNC_INFO
//                     << "role:" << role << "is now connected to: "
//                     << "SLOT" << receiver << QString("(%1)").arg(QString(method))
//                     << "SIGNAL" << sender << QString("(%1)").arg(QString(signal));
    }
    else
    {
        _sender = NULL;
        _signal = NULL;
        connect(this, SIGNAL(toggled(bool)), this, SLOT(setFeedback(bool)));
//            qDebug() << Q_FUNC_INFO
//                     << "role:" << role << "is now connected to: "
//                     << "SLOT" << receiver << QString("(%1)").arg(QString(method))
//                     << "SIGNAL" << trigger << QString("(%1)").arg(QString(SIGNAL(toggled(bool))));
    }
}

void MinoTrigger::disconnectFeedback()
{
    if(_sender && _signal)
    {
        disconnect(_sender, _signal, this, SLOT(setFeedback(bool)));
        _sender = NULL;
        _signal = NULL;
    }
}
