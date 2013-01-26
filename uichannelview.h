#ifndef UICHANNELPREVIEW_H
#define UICHANNELPREVIEW_H

#include <QWidget>

#include "minochannel.h"

class UiChannelView : public QWidget
{
    Q_OBJECT
public:
    explicit UiChannelView(MinoChannel *channel, QWidget *parent);
signals:
protected:
        void paintEvent(QPaintEvent *event);
        virtual int heightForWidth( int width ) const;
public slots:
private:
    MinoChannel *_channel;
};

#endif // UICHANNELPREVIEW_H
