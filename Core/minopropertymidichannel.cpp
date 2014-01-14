#include "minopropertymidichannel.h"

MinoPropertyMidiChannel::MinoPropertyMidiChannel(QObject *parent) :
    MinoProperty(parent),
    _channel(0) // Default: disabled
{
    setObjectName("channel");
    setLabel("Channel");
}

void MinoPropertyMidiChannel::setChannel(const int channel)
{
    if(_channel != channel)
    {
        _channel = channel;
        emit channelChanged(channel);
    }
}
