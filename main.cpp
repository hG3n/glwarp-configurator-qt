#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QResource>
#include <QFile>
#include <QDir>
#include <QDebug>

#include "ui/mainwindow.h"

const QString loadFileToString(const QString &path);

int main(int argc, char *argv[])
{
    // create application
    QApplication app(argc, argv);

    // register resources
    QResource::registerResource("/_RES/resource.qrc");

    // load style
    QString style = loadFileToString(":/styles/style.css");
    app.setStyleSheet(style);

    // gl-options
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    // create main window
    MainWindow mainWindow;
    mainWindow.resize(mainWindow.sizeHint());
    int desktopArea = QApplication::desktop()->width() * QApplication::desktop()->height();

    // check widget area
    int widgetArea = mainWindow.width() * mainWindow.height();
    if ( ((float)widgetArea / (float)desktopArea) < 0.75f )
        mainWindow.show();
    else
        mainWindow.showMaximized();
    return app.exec();
}

const QString loadFileToString(const QString &path)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly)){
        return QLatin1String(file.readAll());
    }
    qDebug() << file.errorString();
    qDebug() << " > " << path;
    qDebug() << QDir::currentPath();
    return QString("");
}
