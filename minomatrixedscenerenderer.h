#ifndef MINOMATRIXEDSCENERENDERER_H
#define MINOMATRIXEDSCENERENDERER_H

#include <QObject>

#include <QGraphicsScene>
#include <QSize>
#include <QRect>
#include <QImage>

class MinoMatrixedSceneRenderer : public QObject
{
    Q_OBJECT
public:
    explicit MinoMatrixedSceneRenderer(QGraphicsScene *scene, QObject *parent = 0);
    ~MinoMatrixedSceneRenderer();

    void setMatrixSize(const QSize matrixSize);
    const QSize matrixSize() { return _matrixSize; }

    void setViewRect(const QRect viewRect) { _viewRect = viewRect; }
    const QRect viewRect() { return _viewRect; }

    const QImage *rendering() { return _image; }

    int heightForWidth( int width ) const { return (qreal)width * _matrixRatio; }

signals:
    void updated();
public slots:
    void render();
protected:
    QGraphicsScene *_scene;
    QSize _matrixSize;
    qreal _matrixRatio;
    QRect _viewRect;
    QImage *_image;
};

#endif // MINOMATRIXEDSCENERENDERER_H
