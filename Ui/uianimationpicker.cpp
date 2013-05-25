#include "uianimationpicker.h"

#include <QLayout>
#include <QPainter>
#include <QDebug>

#include "minopersistentobjectfactory.h"

#include "minoanimation.h"
#include "uianimationdescription.h"

UiAnimationPicker::UiAnimationPicker(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout *lMain = new QVBoxLayout(this);
    setLayout(lMain);
    lMain->setSpacing(0);
    lMain->setMargin(0);
    lMain->setContentsMargins(0,0,0,0);
    QWidget *wContent = new QWidget(this);
    wContent->setObjectName("scrollbackground");
    this->layout()->addWidget(wContent);
    QHBoxLayout *lContent = new QHBoxLayout(wContent);
    lContent->setSpacing(10);

    this->setAttribute(Qt::WA_TranslucentBackground,false);

    QList<MinoAnimationDescription> animations = MinoPersistentObjectFactory::availableAnimationModels();
    foreach(MinoAnimationDescription animation, animations)
    {
        UiAnimationDescription *ad = new UiAnimationDescription(animation, wContent);
        ad->setObjectName("image");
        lContent->addWidget(ad);
    }
    lContent->addStretch();

}

void UiAnimationPicker::mousePressEvent(QMouseEvent *event)
{
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
     mimeData->setData("application/x-dnd_minoanimationdescription", itemData);

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
