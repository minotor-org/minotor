#include "minomaster.h"

#include "minotor.h"
#include "minochannel.h"

#include <QGraphicsProxyWidget>
#include <QDebug>

MinoMaster::MinoMaster(MinoChannel *channel1, MinoChannel *channel2, QObject *parent) :
    QObject(parent),
    _channel1(channel1),
    _channel2(channel2)
{
    // TODO Remove hardcoded values
    _scene.setSceneRect(QRectF(0, 0, 480, 160));
    _scene.addItem(&_itemGroup);

    _view.setScene(&_scene);

    _view.setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    _view.setTransformationAnchor(QGraphicsView::NoAnchor);
    _view.setResizeAnchor(QGraphicsView::NoAnchor);
    _view.setInteractive(false);

    _view.viewport()->setFixedSize(240,160);
    _view.setFixedSize(240,160);
    _view.setSceneRect(QRectF(0, 0, 240, 160));
    _view.fitInView(_view.sceneRect());

    _view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view.setFrameShape(QFrame::NoFrame);
    _view.setStyleSheet("background:transparent;");
    _view.setAttribute(Qt::WA_TranslucentBackground);
    _view.setWindowFlags(Qt::FramelessWindowHint);


    QGraphicsProxyWidget* channel1View = _scene.addWidget(channel1->view());
    channel1View->setGeometry(QRectF(0.0, 0.0, 240.0, 160.0));
    //channel1View->setOpacity(0.5);
    _itemGroup.addToGroup(channel1View);
    channel1View->setVisible(true);

    QGraphicsProxyWidget* channel2View = _scene.addWidget(channel2->view());
    channel2View->setGeometry(QRectF(240.0, 0.0, 240.0, 160.0));
    //channel2View->setOpacity(0.5);
    channel2View->setVisible(true);
    _itemGroup.addToGroup(channel2View);
}

MinoMaster::~MinoMaster()
{

}

void MinoMaster::valueToViewPort(int value)
{
    qreal rValue = ((qreal)value)/127;
    //qDebug() << "rValue" << rValue;
    _view.setSceneRect(QRectF(240.0*rValue, 0, 240, 160));
    _view.fitInView(_view.sceneRect());
}

void MinoMaster::setBrightness(qreal value)
{
    qDebug() << "brightness" << value;
    _itemGroup.setOpacity(value);
}
