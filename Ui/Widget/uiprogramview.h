#ifndef UIPROGRAMVIEW_H
#define UIPROGRAMVIEW_H

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
    void resizeEvent(QResizeEvent *);

    virtual int heightForWidth( int width ) const;
public slots:
    void setProgram(MinoProgram *program);
protected slots:
    void clear();
private:
    MinoProgram *_program;
    QVector<QLine> _gridLines;
    qreal _gridStepMin;

};

#endif // UIPROGRAMVIEW_H
