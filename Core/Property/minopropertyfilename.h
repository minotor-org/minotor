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

#ifndef MINOPROPERTYFILENAME_H
#define MINOPROPERTYFILENAME_H

#include <QString>

#include "minoproperty.h"

class MinoPropertyFilename : public MinoProperty
{
    Q_OBJECT

    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged STORED true)

public:
    explicit MinoPropertyFilename(QObject *parent = 0);

    QString filename() { return _filename; }

public slots:
    void setFilename(const QString& filename);

signals:
    void filenameChanged(const QString& filename);

protected:
    QString _filename;

};
#endif // MINOPROPERTYFILENAME_H
