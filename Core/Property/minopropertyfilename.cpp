#include "minopropertyfilename.h"

MinoPropertyFilename::MinoPropertyFilename(QObject *parent) :
    MinoProperty(parent)
{
    setObjectName("filename");
    setLabel("Filename");
}

void MinoPropertyFilename::setFilename(const QString& filename)
 {
    if(_filename != filename)
    {
        _filename = filename;
        emit filenameChanged(filename);
    }
 }
