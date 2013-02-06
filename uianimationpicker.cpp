#include "uianimationpicker.h"

#include <QLayout>
#include <QPainter>
#include <QDebug>

#include "minoanimationfactory.h"

#include "minoanimation.h"
#include "uianimationdescription.h"

UiAnimationPicker::UiAnimationPicker(QWidget *parent) :
    QWidget(parent)
{
    setLayout(new QHBoxLayout);
    QWidget *wContent = new QWidget(this);
    wContent->setObjectName("scrollbackground");
    this->layout()->addWidget(wContent);
    QHBoxLayout *lContent = new QHBoxLayout(wContent);

    this->setAttribute(Qt::WA_TranslucentBackground,false);

    QList<MinoAnimationDescription> animations = MinoAnimationFactory::availableAnimations();
    foreach(MinoAnimationDescription animation, animations)
    {
        UiAnimationDescription *ad = new UiAnimationDescription(animation, wContent);
        ad->setObjectName("image");
        lContent->addWidget(ad);
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
     dataStream << child->description()->className() << QPoint(event->pos() - child->pos());

     QMimeData *mimeData = new QMimeData;
     mimeData->setData("application/x-dndanimationdescrition", itemData);

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
