#ifndef UICHANNEL_H
#define UICHANNEL_H

#include <QWidget>
#include <QAction>

#include "minochannel.h"

class UiChannel : public QWidget
{
    Q_OBJECT
public:
    explicit UiChannel(MinoChannel *channel, QWidget *parent);
    ~UiChannel();

signals:
    
public slots:

private slots:


private:
    MinoChannel *_channel;
};

#endif // UICHANNEL_H
