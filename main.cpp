#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // HACK
    qDebug() << "";
    // Load an application style
    QFile styleFile( "style.css" );
    styleFile.open( QFile::ReadOnly );

    // Apply the loaded stylesheet
    QString style( styleFile.readAll() );
    a.setStyleSheet( style );
    // a.setOrganizationName(); ???
    a.setApplicationName("Minotor");
    MainWindow w;

    w.show();

    return a.exec();
}
