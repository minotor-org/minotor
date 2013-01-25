#include "minomatrixedscenerenderer.h"

#include <QPainter>
#include <QDebug>

MinoMatrixedSceneRenderer::MinoMatrixedSceneRenderer(QGraphicsScene *scene, QObject *parent) :
    QObject(parent),
    _scene(scene)
{
    // Set default values
    _image = NULL;
    setMatrixSize(QSize(32, 24));
    setViewRect(QRect(0, 0, 32, 24));
}

MinoMatrixedSceneRenderer::~MinoMatrixedSceneRenderer()
{
    delete _image;
}

void MinoMatrixedSceneRenderer::render()
{
    // Set background
    _image->fill(Qt::black);

    // Render the scene at previously saved view rect
    QPainter painter(_image);
    _scene->render(&painter, QRectF(_image->rect()), _viewRect, Qt::IgnoreAspectRatio);

    /*
    qDebug() << "MinoMatrixedSceneRenderer"
             << "painter" << painter.viewport()
             << "view rect:" << _viewRect
             << "scene rect:" << _scene->sceneRect();
    */
    emit updated();
}


void MinoMatrixedSceneRenderer::setMatrixSize(const QSize matrixSize)
{
    _matrixSize = matrixSize;
    _matrixRatio = (qreal)_matrixSize.height() / (qreal)_matrixSize.width();
    if (_image) delete _image;
    _image = new QImage(_matrixSize, QImage::Format_RGB32);
}
