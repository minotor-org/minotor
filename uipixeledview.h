#ifndef UICHANNELPREVIEW_H
#define UICHANNELPREVIEW_H

#include <QFrame>
#include <QWidget>
#include <QGraphicsView>

class UiPixeledView : public QWidget
{
    Q_OBJECT
public:
    explicit UiPixeledView(QGraphicsView *view, QWidget *parent);
    ~UiPixeledView();
signals:
protected:
        void paintEvent(QPaintEvent *event);
        virtual int heightForWidth( int width ) const;
public slots:
    void refresh();

private:
    QGraphicsView *_view;
    QImage *_image;
};

#endif // UICHANNELPREVIEW_H
