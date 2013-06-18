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

#ifndef MINAIMAGE_H
#define MINAIMAGE_H

#include "minoanimation.h"

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QImageReader>

#include "minopropertyeasingcurve.h"
#include "minopropertyfilename.h"

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = 0);

    void setImage(QImage *image);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);

private:
    QImage _image;
};

class MinaImage : public MinoAnimation
{
    Q_OBJECT

public:
    explicit MinaImage(QObject *parent);
    ~MinaImage();

    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Image", "Use file as image source", QPixmap(), MinaImage::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }
    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:
    
public slots:
    void loadFromFile(const QString& filename);

private:
    QGraphicsItemGroup _itemGroup;
    ImageWidget *_imageWidget;
    QList<QImage*> _imageList;
    int _imageIndex;
    MinoPropertyEasingCurve *_generatorCurve;
    MinoPropertyFilename *_imageFilename;

};

#endif // MINAIMAGE_H
