#include "minotor.h"

#include <QtCore/QDebug>

#include <QGraphicsView>
//#include <QColor>
//#include <QRgb>
//#include <QPainter>

Minotor::Minotor(Midi *midi, QObject *parent) :
    QObject(parent),
    _ppqnId(0),
    _isSequenceRunning(false),
    _ledMatrix(NULL)
{
    _scene.setSceneRect(QRectF(0, 0, 240, 160));
    _scene.addItem(&_mainItemGroup);

    QRadialGradient gradient(130, 130, 50, 130, 130);
    gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 1));
    QBrush brush(gradient);


    _mainItemGroup.addToGroup(_scene.addRect(QRectF(90, 50, 60, 60), QColor::fromRgbF(0.5, 0, 1, 1)));
    _mainItemGroup.addToGroup(_scene.addRect(QRectF(100, 60, 40, 40), QColor::fromRgbF(0, 0.5, 1, 1)));



    //QPixmap background;
    //background.fill(Qt::black);
    //ui->graphicsViewMatrix->setScene(&_scene);
    //ui->graphicsViewMatrix->show();

    _animation.setStartValue(QVariant(3.0));
    _animation.setEndValue(QVariant(0.1));
    //_animation.setEasingCurve(QEasingCurve::InBounce);

    connect(midi,SIGNAL(clockReceived()),this,SLOT(handleClock()));
    connect(midi,SIGNAL(startReceived()),this,SLOT(handleStart()));
    connect(midi,SIGNAL(stopReceived()),this,SLOT(handleStop()));
    connect(midi,SIGNAL(continueReceived()),this,SLOT(handleContinue()));
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
            static qreal currentScale = 1.0;

            const int currentTime = (qreal(_animation.duration())) * (((qreal)_ppqnId) / 24.0);
            _animation.setCurrentTime(currentTime);
            _mainItemGroup.setTransformOriginPoint(120,80);
            _mainItemGroup.setScale(_animation.currentValue().toReal());
            //foreach(QGraphicsItem* item, _mainItemGroup.childItems ())
            //{
                //item->rotate((360/12)/4);
            //}

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

