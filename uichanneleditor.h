#ifndef UICHANNELEDITOR_H
#define UICHANNELEDITOR_H

#include <QWidget>
#include <QAction>

#include "minochannel.h"

class UiChannelEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UiChannelEditor(MinoChannel *channel, QWidget *parent);
    ~UiChannelEditor();

signals:
    
public slots:

private slots:


private:
    MinoChannel *_channel;
};

#endif // UICHANNELEDITOR_H
