#ifndef MINOMASTER_H
#define MINOMASTER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItemGroup>

class MinoMaster : public QObject
{
    Q_OBJECT
public:
    explicit MinoMaster(QObject *parent = 0);
    ~MinoMaster();
    QGraphicsScene *scene() { return &_scene; }
    QGraphicsView *view() { return &_view; }
    QGraphicsItemGroup *itemGroup() { return &_itemGroup; }
private:
    QGraphicsScene _scene;
    QGraphicsView _view;
    QGraphicsItemGroup _itemGroup;
signals:
    
public slots:
    
};

#endif // MINOMASTER_H
