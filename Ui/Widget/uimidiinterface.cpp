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

#include "uimidiinterface.h"

#include <QLayout>
#include <QDebug>

#include "configdialog.h"
#include "midimapper.h"

UiMidiInterface::UiMidiInterface(MidiInterface *interface, QWidget *parent) :
    QWidget(parent),
    _interface(interface)
{
    //Item 1
    QHBoxLayout *lMaster = new QHBoxLayout(this);
    this->setLayout(lMaster);

    //connection state
    QPushButton *_pbConnection = new QPushButton("",this);
    _pbConnection->setObjectName("tiny");
    _pbConnection->setCheckable(true);
    _pbConnection->setChecked(_interface->isConnected());
    _pbConnection->setEnabled(false);
    _pbConnection->setFixedSize(9,9);
    connect(_interface,SIGNAL(connected(bool)),_pbConnection,SLOT(setChecked(bool)));
    this->layout()->addWidget(_pbConnection);

    //Name
    _txtName = new QLabel(this);
    _txtName->setObjectName("interfacename");
    _txtName->setText(_interface->portName() + QString("    ID: ") + QString::number(_interface->id()));
    this->layout()->addWidget(_txtName);
    dynamic_cast<QHBoxLayout*>(this->layout())->addStretch(1);
    //Mapping : fake data
    _cbMapping = new QComboBox(this);
    _cbMapping->setToolTip("Select midi mapping to use with this interface");
    ConfigDialog* cd = qobject_cast<ConfigDialog*>(parent);
    cd->loadMidiMappingFiles(_cbMapping);
    int index = _cbMapping->findData(QVariant(_interface->mapping()));
    if(index == -1)
    {
        if(!_interface->mapping().isEmpty())
            qDebug() << Q_FUNC_INFO
                 << "mapping not found:" << _interface->mapping();

        // Select "none"
        _cbMapping->setCurrentIndex(0);
    }
    else
    {
        _cbMapping->setCurrentIndex(index);
    }

    connect(_cbMapping, SIGNAL(currentIndexChanged(int)),this,SLOT(mappingChanged(int)));

    this->layout()->addWidget(_cbMapping);
}

void UiMidiInterface::mappingChanged(int mapping)
{
   _interface->setMapping(_cbMapping->itemData(mapping).toString());

}
