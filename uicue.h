#ifndef UICUE_H
#define UICUE_H

#include <QWidget>

#include "minocue.h"

class UiCue : public QWidget
{
    Q_OBJECT
public:
    explicit UiCue(MinoCue *cue, QWidget *parent);
    
signals:
    
public slots:

private:
MinoCue *_cue;
};

#endif // UICUE_H
