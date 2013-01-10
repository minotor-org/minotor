#include "uianimation.h"
#include <QLayout>
#include <QDebug>
#include <mainwindow.h>
#include <QMenu>

UiAnimation::UiAnimation(MinoAnimation *animation, QWidget *parent) :
    QWidget(parent)
{
    qDebug() << "New UiAnimation for" << animation->objectName();
    QVBoxLayout *layout = new QVBoxLayout(this);
    //layout->se()
    QLabel *lbl = new QLabel(animation->name());
    lbl->setAlignment(Qt::AlignCenter);
    layout->addWidget(lbl);

    foreach (MinoAnimationProperty *property, animation->properties())
    {
        UiDial *d = new UiDial(property, this);
        connect(d, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
        layout->addWidget(d);
        QLabel *l = new QLabel(QString(property->objectName()));
        l->setAlignment(Qt::AlignCenter);
        layout->addWidget(l);

        qDebug() << "New property for" << property->objectName();
    }
    layout->addStretch();
}

