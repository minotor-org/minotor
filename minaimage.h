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
    ImageWidget(QWidget *parent = 0);

    void setImage(QImage *image);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *event);

private:
    QImage _image;
};

class MinaImage : public MinoAnimation
{
    Q_OBJECT

public:
    explicit MinaImage(MinoAnimationGroup *parent);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Image", "Use file as image source", QPixmap(), MinaImage::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaImage::getDescription(); }
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
