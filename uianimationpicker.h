#ifndef UIANIMATIONPICKER_H
#define UIANIMATIONPICKER_H

#include <QWidget>
#include <QMouseEvent>

class UiAnimationPicker : public QWidget
{
    Q_OBJECT
public:
    explicit UiAnimationPicker(QWidget *parent);
    
protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    
public slots:
    
};

#endif // UIANIMATIONPICKER_H
