#include "uianimationproperty.h"

#include <QDebug>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>

#include "minotor.h"

#include "minopropertytext.h"
#include "minopropertyfilename.h"
#include "minopropertyeasingcurve.h"

#include "uimidicontrollableparameter.h"
#include "uieasingcurve.h"

UiAnimationProperty::UiAnimationProperty(MinoProperty *property, QWidget *parent, bool editorMode) :
    QWidget(parent),
    _columnCount(0)
{
    this->setMinimumWidth(50);
    this->setMinimumHeight(50);
    this->setMaximumHeight(50);

    QHBoxLayout *lProperty = new QHBoxLayout(this);
    lProperty->setSpacing(5);
    lProperty->setMargin(0);
    lProperty->setContentsMargins(0,0,0,0);

    foreach(MidiControllableParameter *param, property->findChildren<MidiControllableParameter*>())
    {
        lProperty->addWidget(new UiMidiControllableParameter(param, this, editorMode));
        _columnCount++;
    }
    if(MinoPropertyEasingCurve *easingCurveProperty = qobject_cast<MinoPropertyEasingCurve*>(property))
    {
        UiEasingCurve * ec = new UiEasingCurve(this);
        ec->setEasingCurve(QEasingCurve(easingCurveProperty->easingCurveType()));
        connect(easingCurveProperty,SIGNAL(easingCurveChanged(QEasingCurve)),ec,SLOT(setEasingCurve(QEasingCurve)));
        lProperty->addWidget(ec);
        _columnCount++;
    }

    if (MinoPropertyText* textProperty = qobject_cast<MinoPropertyText*>(property))
    {
        QLineEdit *leText = new QLineEdit(this);
        leText->setObjectName("textedit");
        leText->setAlignment(Qt::AlignHCenter);
        leText->setText(textProperty->text());
        connect(leText, SIGNAL(textChanged(QString)), textProperty, SLOT(setText(QString)));
        lProperty->addWidget(leText);
        _columnCount+=2;
    }
    else if (MinoPropertyFilename *filenameProperty = qobject_cast<MinoPropertyFilename*>(property))
    {
        QPushButton *pbLoad = new QPushButton(this);
        lProperty->addWidget(pbLoad);
        pbLoad->setIcon(QIcon(":/pictos/load.png"));
        pbLoad->setIconSize(QSize(8,8));
        pbLoad->setMinimumSize(12,12);
        pbLoad->setMaximumSize(12,12);
        QFileDialog *fdLoad = new QFileDialog(this);
        connect(pbLoad,SIGNAL(clicked()),fdLoad,SLOT(open()));
        connect(fdLoad,SIGNAL(fileSelected(QString)),filenameProperty,SLOT(setFilename(QString)));

        QLabel *label = new QLabel(this);
        label->setObjectName("filename");
        label->setText(filenameProperty->filename());
        lProperty->addWidget(label);
        _columnCount+=2;
    }
}

int UiAnimationProperty::columnCount () const
{
    return _columnCount;
}
