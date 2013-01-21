#ifndef UICHANNELPREVIEW_H
#define UICHANNELPREVIEW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QSize>
#include <QRect>

class UiPixeledView : public QWidget
{
    Q_OBJECT
public:
    explicit UiPixeledView(QGraphicsScene *scene, QWidget *parent);

    void setMatrixSize(const QSize matrixSize);
    const QSize matrixSize() { return _matrixSize; }

    void setViewRect(const QRect viewRect) { _viewRect = viewRect; }
    const QRect viewRect() { return _viewRect; }

    ~UiPixeledView();
signals:
protected:
        void paintEvent(QPaintEvent *event);
        virtual int heightForWidth( int width ) const;
public slots:
    void refresh();

private:
    QSize _matrixSize;
    qreal _matrixRatio;
    QRect _viewRect;
    QGraphicsScene *_scene;
    QImage *_image;
};

#endif // UICHANNELPREVIEW_H
