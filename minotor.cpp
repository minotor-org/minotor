#include "minotor.h"

#include <QtCore/QDebug>

#include <QGraphicsView>
//#include <QColor>
//#include <QRgb>

#include "minoanimation.h"
#include "minoanimationrandompixels.h"
#include "minoanimationexpandingobjects.h"

Minotor::Minotor(Midi *midi, QObject *parent) :
    QObject(parent),
    _ppqnId(0),
    _isSequenceRunning(false),
    _ledMatrix(NULL)
{
    _scene.setSceneRect(QRectF(0, 0, 240, 160));
    _scene.addItem(&_mainItemGroup);

    _minoAnimations.append(new MinoAnimationRandomPixels(this));
    _minoAnimations.append(new MinoAnimationExpandingObjects(this));

//    QRadialGradient gradient(130, 130, 50, 130, 130);
//    gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
//    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 1));
//    QBrush brush(gradient);


    foreach(MinoAnimation *minoAnimation, _minoAnimations)
    {
        _mainItemGroup.addToGroup(minoAnimation->itemGroup());
    }

    _midiInterfaces.append(midi);
    // Connections to Midi manager
    connect(midi,SIGNAL(clockReceived()),this,SLOT(handleClock()));
    connect(midi,SIGNAL(startReceived()),this,SLOT(handleStart()));
    connect(midi,SIGNAL(stopReceived()),this,SLOT(handleStop()));
    connect(midi,SIGNAL(continueReceived()),this,SLOT(handleContinue()));
    connect(midi,SIGNAL(controlChanged(quint8,quint8,quint8)),this,SLOT(handleMidiInterfaceControlChange(quint8,quint8,quint8)));

    // Link Minotor to MidiMapping
    connect(this, SIGNAL(controlChanged(int,quint8,quint8,quint8)), &_midiMapping, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
}

QGraphicsScene* Minotor::scene()
{
    return &_scene;
}

void Minotor::setLedMatrix(LedMatrix *ledMatrix)
{
    _ledMatrix = ledMatrix;
}

void Minotor::handleClock(void)
{
    if(_isSequenceRunning)
    {
        /*
        // Flash
        if ((nbClock%24)==0)
        {
           _ledMatrix->fill(Qt::white);
           _ledMatrix->show();
        }
        else if ((nbClock%24) ==1)
        {
            _ledMatrix->fill(Qt::black);
            _ledMatrix->show();
        }
        */

        if((_ppqnId%2) == 0) {
            foreach(MinoAnimation *minoAnimation, _minoAnimations)
            minoAnimation->animate(_ppqnId);
            _ledMatrix->showScene(&_scene);
        }
/*
        const int currentTime = (qreal(animation.duration())) * (((qreal)nbClock) / 24.0);
        animation.setCurrentTime(currentTime);
        static int currentAngle = 0;
        const int delta = animation.currentValue().toInt() - currentAngle;
        ui->graphicsView->rotate(delta);
        currentAngle += delta;
        _ledMatrix->showView(ui->graphicsView);
*/
        /*
        // if ((nbClock%24)==0) {
             ui->graphicsView->rotate(1);
             _ledMatrix->showView(ui->graphicsView);
        // }
        */

        // Clock counter
        if (_ppqnId==23) { _ppqnId = 0; } else { _ppqnId++; }
    }
}

void Minotor::handleStop(void)
{
    _isSequenceRunning = false;
}

void Minotor::handleStart(void)
{
    _ppqnId = 0;
    _isSequenceRunning = true;
}

void Minotor::handleContinue(void)
{
    _isSequenceRunning = true;
}

/*
#define KORG_FX_EDIT1 92
void Minotor::handleMidiInterfaceControlChange(quint8 control, quint8 value)
{
    switch (control) {
    case KORG_FX_EDIT1:
        emit colorControlChanged(value);
        break;
    default:
        qDebug() << "unhandled control change: " << control << "( value" << value << ")";
    }
}
*/

void Minotor::handleMidiInterfaceControlChange(quint8 channel, quint8 control, quint8 value)
{
    // qDebug() << "sender:" << QObject::sender()->metaObject()->className();

    Midi *midiInterface = ((Midi*)QObject::sender());
    if (_midiInterfaces.contains(midiInterface))
    {
        const int midiInterfaceId = _midiInterfaces.indexOf(midiInterface);
        emit(controlChanged(midiInterfaceId, channel, control, value));
    } else {
        qDebug() << "Unknow sender";
    }

}
