#ifndef UICHANNELVIEW_H
#define UICHANNELVIEW_H

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

#endif // UICHANNELVIEW_H
