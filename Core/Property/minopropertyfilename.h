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
