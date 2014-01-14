#ifndef MINOPROPERTYMIDICHANNEL_H
#define MINOPROPERTYMIDICHANNEL_H

#include "minoproperty.h"

class MinoPropertyMidiChannel : public MinoProperty
{
    Q_OBJECT

    Q_PROPERTY(int channel READ channel WRITE setChannel NOTIFY channelChanged STORED true)

public:
    explicit MinoPropertyMidiChannel(QObject *parent);

    /* return listened channel (0:none, 1..15 channel) */
    int channel() { return _channel; }

public slots:
    void setChannel(const int channel);

signals:
    void channelChanged(const int channel);

protected:
    int _channel;

};

#endif // MINOPROPERTYMIDICHANNEL_H
