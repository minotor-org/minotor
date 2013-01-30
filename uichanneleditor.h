#ifndef UICHANNELEDITOR_H
#define UICHANNELEDITOR_H

#include <QWidget>

#include <QDragEnterEvent>
#include <QDropEvent>

#include "minochannel.h"

class UiChannelEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UiChannelEditor(MinoChannel *channel, QWidget *parent);
    ~UiChannelEditor();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    QWidget *_wContent;

    void addAnimation(MinoAnimation *animation);
signals:
    
public slots:

private slots:


private:
    MinoChannel *_channel;
};

#endif // UICHANNELEDITOR_H
