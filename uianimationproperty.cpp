#include "uianimationproperty.h"

#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QLayout>
#include <QLineEdit>

#include "minotor.h"

#include "minoitemizedproperty.h"
#include "minotextproperty.h"

UiAnimationProperty::UiAnimationProperty(MinoProperty *property, QWidget *parent) :
    UiMidiProperty(property, parent)
{
    if(this->layout())
        delete this->layout();
    this->setMinimumWidth(45);
    this->setMaximumWidth(45);
    QVBoxLayout *lProperty = new QVBoxLayout(this);
    lProperty->setSpacing(0);
    lProperty->setMargin(0);
    lProperty->setContentsMargins(0,0,0,0);

    lProperty->addStretch();
    MinoTextProperty* textProperty = dynamic_cast<MinoTextProperty*>(property);
    if(textProperty)
    {
        QLineEdit *teText = new QLineEdit(this);
        teText->setObjectName("textedit");
        teText->setAlignment(Qt::AlignHCenter);
        connect(teText, SIGNAL(textChanged(QString)), textProperty, SLOT(setTextValue(QString)));
        lProperty->addWidget(teText);
    }
    else
    {
        QWidget *wTop = new QWidget(this);
        lProperty->addWidget(wTop);
        wTop->setFixedHeight(12);
        QHBoxLayout *lTop = new QHBoxLayout(wTop);
        lTop->setSpacing(0);
        lTop->setMargin(0);
        lTop->setContentsMargins(2,0,2,0);
        QWidget *wLeft = new QWidget(wTop);
        wLeft->setMinimumSize(6,6);
        wLeft->setMaximumSize(6,6);
        lTop->addWidget(wLeft);
        lTop->addStretch();
        MinoItemizedProperty* itemizedProperty = dynamic_cast<MinoItemizedProperty*>(property);
        if(itemizedProperty)
        {
            QLabel *tItemName = new QLabel(itemizedProperty->currentItem()->name(), wTop);
            tItemName->setObjectName("dialinfo");
            tItemName->setAlignment(Qt::AlignHCenter);
            connect(itemizedProperty, SIGNAL(itemChanged(QString)), tItemName, SLOT(setText(QString)));
            lTop->addWidget(tItemName);
        }
        lTop->addStretch();
        QPushButton *pbOnMaster = new QPushButton(wTop);
        pbOnMaster->setObjectName("tiny");
        pbOnMaster->setCheckable(true);
        pbOnMaster->setFixedSize(6,6);
        lTop->addWidget(pbOnMaster);
        connect(pbOnMaster,SIGNAL(toggled(bool)), this, SLOT(togglePropertyToMaster(bool)));

        QWidget *wDial = new QWidget(this);
        lProperty->addWidget(wDial);
        QHBoxLayout *lDial = new QHBoxLayout(wDial);
        lDial->setSpacing(0);
        lDial->setMargin(0);
        lDial->setContentsMargins(0,0,0,0);
        lDial->addStretch();
        _knob->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        lDial->addWidget(_knob);
        lDial->addStretch();

        QLabel *t = new QLabel(QString(property->objectName()), this);
        t->setObjectName("dialinfo");
        t->setAlignment(Qt::AlignHCenter);
        lProperty->addWidget(t);
    }
}

void UiAnimationProperty::togglePropertyToMaster(bool on)
{
    if (on)
    {
        _property->setAttributes(_property->attributes() | MinoProperty::Important);
    }
    else
    {
        _property->setAttributes(_property->attributes() & ~MinoProperty::Attributes(MinoProperty::Important));
    }
}
