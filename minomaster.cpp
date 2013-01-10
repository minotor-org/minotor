#include "minomaster.h"

MinoMaster::MinoMaster(QObject *parent) :
    QObject(parent)
{
    // TODO Remove hardcoded values
    //_scene.setSceneRect(QRectF(0, 0, 480, 160));
    _scene.addItem(&_itemGroup);

    _view.setScene(&_scene);
    _view.setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    _view.setTransformationAnchor(QGraphicsView::NoAnchor);
    _view.setResizeAnchor(QGraphicsView::NoAnchor);
    _view.setInteractive(false);
    _view.viewport()->setFixedSize(240,160);
    _view.setFixedSize(240,160);
    //_view.setGeometry(0,0,240,160);
    _view.setSceneRect(QRectF(80, 80, 240*2, 160*2));
    _view.fitInView(_view.sceneRect());
    //_view.fitInView(QRectF(80, 80, 240*2, 160*2));
    //_view.resize(240,160);
    _view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view.setFrameShape(QFrame::NoFrame);
    _view.setStyleSheet("background:transparent;");
    _view.setAttribute(Qt::WA_TranslucentBackground);
    _view.setWindowFlags(Qt::FramelessWindowHint);
}

MinoMaster::~MinoMaster()
{

}
