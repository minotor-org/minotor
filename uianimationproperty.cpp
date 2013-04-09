#include "uianimationproperty.h"

#include <QDebug>
#include <QLayout>
#include <QLineEdit>

#include "minotor.h"

#include "minopropertytext.h"
#include "uimidicontrollableparameter.h"

UiAnimationProperty::UiAnimationProperty(MinoProperty *property, QWidget *parent, bool editorMode) :
    QWidget(parent),
    _columnCount(0)
{
    this->setMinimumWidth(45);
    this->setMinimumHeight(45);
    this->setMaximumHeight(45);

    QHBoxLayout *lProperty = new QHBoxLayout(this);
    lProperty->setSpacing(0);
    lProperty->setMargin(0);
    lProperty->setContentsMargins(0,0,0,0);

    MinoPropertyText* textProperty = dynamic_cast<MinoPropertyText*>(property);

    foreach(MidiControllableParameter *param, property->findChildren<MidiControllableParameter*>())
    {
        lProperty->addWidget(new UiMidiControllableParameter(param, this, editorMode));
        _columnCount++;
    }

    if (textProperty)
    {
        QLineEdit *leText = new QLineEdit(this);
        leText->setObjectName("textedit");
        leText->setAlignment(Qt::AlignHCenter);
        leText->setText(textProperty->text());
        connect(leText, SIGNAL(textChanged(QString)), textProperty, SLOT(setText(QString)));
        lProperty->addWidget(leText);
        _columnCount+=2;
    }
}

int UiAnimationProperty::columnCount () const
{
    return _columnCount;
}
