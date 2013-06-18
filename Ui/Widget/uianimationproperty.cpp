/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        QWidget *wText = new QWidget(this);
        QVBoxLayout *lText = new QVBoxLayout(wText);

        QLabel *label = new QLabel(wText);
        label->setObjectName("textname");
        label->setText("text");
        label->setAlignment(Qt::AlignCenter);
        lText->addWidget(label);

        QLineEdit *leText = new QLineEdit(this);
        leText->setObjectName("textedit");
        leText->setAlignment(Qt::AlignCenter);
        leText->setText(textProperty->text());
        connect(leText, SIGNAL(textChanged(QString)), textProperty, SLOT(setText(QString)));
        lText->addWidget(leText);

        lProperty->addWidget(wText);
        _columnCount+=2;
    }
    else if (MinoPropertyFilename *filenameProperty = qobject_cast<MinoPropertyFilename*>(property))
    {
        QPushButton *pbLoad = new QPushButton(this);
        lProperty->addWidget(pbLoad);
        pbLoad->setText("Load");
        pbLoad->setMinimumSize(50,26);
        pbLoad->setMaximumSize(50,26);
        QFileDialog *fdLoad = new QFileDialog(this);
        connect(pbLoad,SIGNAL(clicked()),fdLoad,SLOT(open()));
        connect(fdLoad,SIGNAL(fileSelected(QString)),filenameProperty,SLOT(setFilename(QString)));

        QLabel *label = new QLabel(this);
        label->setObjectName("filename");
        label->setText(filenameProperty->filename());
        lProperty->addWidget(label);
        _columnCount+=3;
    }
}

int UiAnimationProperty::columnCount () const
{
    return _columnCount;
}
