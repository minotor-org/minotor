#include "uianimationpicker.h"

#include <QLayout>
#include <QPainter>
#include <QDebug>

#include "minoanimation.h"
#include "uianimationdescription.h"

UiAnimationPicker::UiAnimationPicker(MinoAnimationFactory *factory, QWidget *parent) :
    QWidget(parent)
{
    setLayout(new QHBoxLayout);
    QList<MinoAnimationDescription> animations = factory->availableAnimations();
    foreach(MinoAnimationDescription animation, animations)
    {
        UiAnimationDescription *ad = new UiAnimationDescription(animation, this);
        layout()->addWidget(ad);
    }
}

void UiAnimationPicker::mousePressEvent(QMouseEvent *event)
{
//    qDebug() << childAt(event->pos())->metaObject()->className();
     UiAnimationDescription *child = dynamic_cast<UiAnimationDescription*>(childAt(event->pos()));
     if (!child)
         return;

     QPixmap pixmap;
     if(!child->pixmap())
     {
         pixmap = QPixmap::grabWidget(child);
     } else {
         pixmap = child->pixmap()->scaled(child->size(), Qt::KeepAspectRatio);
     }

     QByteArray itemData;
     QDataStream dataStream(&itemData, QIODevice::WriteOnly);
     dataStream << pixmap << QPoint(event->pos() - child->pos());

     QMimeData *mimeData = new QMimeData;
     mimeData->setData("application/x-dnditemdata", itemData);

     QDrag *drag = new QDrag(this);
     drag->setMimeData(mimeData);
     drag->setPixmap(pixmap);
     drag->setHotSpot(event->pos() - child->pos());

     QPixmap tempPixmap = pixmap;
     QPainter painter;
     painter.begin(&tempPixmap);
     painter.fillRect(pixmap.rect(), QColor(255, 127, 127, 127));
     painter.end();

     child->setPixmap(tempPixmap);

     if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
         child->close();
     else {
         child->show();
         child->setPixmap(pixmap);
     }
 }
