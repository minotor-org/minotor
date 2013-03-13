#include "uianimationproperty.h"

#include <QDebug>
#include <QLayout>
#include <QLineEdit>

#include "minotor.h"

#include "minotextproperty.h"

UiAnimationProperty::UiAnimationProperty(MinoProperty *property, QWidget *parent, bool editorMode) :
    QWidget(parent)
{
    this->setMinimumWidth(45);
    this->setMinimumHeight(45);
    this->setMaximumHeight(45);

    QHBoxLayout *lProperty = new QHBoxLayout(this);
    lProperty->setSpacing(0);
    lProperty->setMargin(0);
    lProperty->setContentsMargins(0,0,0,0);

    MinoTextProperty* textProperty = dynamic_cast<MinoTextProperty*>(property);
    if(textProperty)
    {
        QLineEdit *teText = new QLineEdit(this);
        teText->setObjectName("textedit");
        teText->setAlignment(Qt::AlignHCenter);
        teText->setText(textProperty->textValue());
        connect(teText, SIGNAL(textChanged(QString)), textProperty, SLOT(setTextValue(QString)));
        lProperty->addWidget(teText);
    }
    else
    {
        lProperty->addWidget(new UiMidiProperty(property, this, editorMode));
    }
}
