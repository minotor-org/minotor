#ifndef UICHANNELVIEW_H
#define UICHANNELVIEW_H

#include <QWidget>

#include "minoprogram.h"

class UiProgramView : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgramView(MinoProgram *program, QWidget *parent);
signals:
protected:
        void paintEvent(QPaintEvent *event);
        virtual int heightForWidth( int width ) const;
public slots:
        void setProgram(MinoProgram *program);
private:
    MinoProgram *_program;
};

#endif // UICHANNELVIEW_H
