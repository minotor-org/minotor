#include "uianimationproperty.h"

#include <QLabel>
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
        MinoItemizedProperty* itemizedProperty = dynamic_cast<MinoItemizedProperty*>(property);
        if(itemizedProperty)
        {
            QLabel *tItemName = new QLabel(itemizedProperty->currentItem()->name(), this);
            tItemName->setObjectName("dialinfo");
            tItemName->setAlignment(Qt::AlignHCenter);
            connect(itemizedProperty, SIGNAL(itemChanged(QString)), tItemName, SLOT(setText(QString)));
            lProperty->addWidget(tItemName);
        }

        QWidget *wDial = new QWidget(this);
        lProperty->addWidget(wDial);
        QHBoxLayout *lDial = new QHBoxLayout(wDial);
        lDial->setSpacing(0);
        lDial->setMargin(0);
        lDial->setContentsMargins(0,0,0,0);
        lDial->addStretch();
        _dial->setMinimumSize(28,28);
        _dial->setMaximumSize(28,28);
        _dial->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        connect(_dial, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
        lDial->addWidget(_dial);
        lDial->addStretch();

        QLabel *t = new QLabel(QString(property->objectName()), this);
        t->setObjectName("dialinfo");
        t->setAlignment(Qt::AlignHCenter);
        lProperty->addWidget(t);
    }
}
