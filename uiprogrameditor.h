#ifndef UICHANNELEDITOR_H
#define UICHANNELEDITOR_H

#include <QWidget>

#include <QDragEnterEvent>
#include <QDropEvent>

#include "minoprogram.h"

class UiProgramEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgramEditor(MinoProgram *program, QWidget *parent);
    ~UiProgramEditor();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    QWidget *_wContent;

    void addAnimation(MinoAnimation *animation);
signals:
    
public slots:
    void setExpanded(bool expanded);
private slots:


private:
    MinoProgram *_program;
    bool _expanded;
};

#endif // UICHANNELEDITOR_H
