#ifndef UICHANNELPREVIEW_H
#define UICHANNELPREVIEW_H

#include <QWidget>

#include "minomatrixedscenerenderer.h"

class UiPixeledView : public QWidget
{
    Q_OBJECT
public:
    explicit UiPixeledView(MinoMatrixedSceneRenderer *renderer, QWidget *parent);
signals:
protected:
        void paintEvent(QPaintEvent *event);
        virtual int heightForWidth( int width ) const;
public slots:
private:
    MinoMatrixedSceneRenderer *_renderer;
};

#endif // UICHANNELPREVIEW_H
