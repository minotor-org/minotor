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
