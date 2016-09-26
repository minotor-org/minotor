/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "uianimationpicker.h"

#include <QLayout>
#include <QPainter>
#include <QDebug>
#include <QMimeData>
#include <QDrag>

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
    foreach(const MinoAnimationDescription& animation, animations)
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
