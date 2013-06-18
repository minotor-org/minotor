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

#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QDebug>

#include "minotor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Auto-create minotor instance
    Minotor::minotor();

    // HACK
    qDebug() << "";
    // Load an application style
    QFile styleFile( "style.css" );
    styleFile.open( QFile::ReadOnly );
    if(!styleFile.isOpen())
    {
        // Switching to internal style.css
        styleFile.setFileName(":/style.css");
        styleFile.open( QFile::ReadOnly );
    }
    // Apply the loaded stylesheet
    QString style( styleFile.readAll() );
    a.setStyleSheet( style );
    // a.setOrganizationName(); ???
    a.setApplicationName("Minotor");

    MainWindow w;
    w.show();

    int ret = a.exec();

    delete Minotor::minotor();

    return ret;
}
