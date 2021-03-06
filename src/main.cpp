#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QResource>
#include <QFile>
#include <QDir>
#include <QDebug>

#include "ui/mainwindow.h"

#include "model/sphere.hpp"
#include "model/ray.hpp"

void test();
const QString loadFileToString(const QString &path);

int main(int argc, char *argv[])
{
    // create application
    QApplication app(argc, argv);
    QResource::registerResource("/_RES/resource.qrc");

    // load style
    QString style = loadFileToString(":/styles/style.css");
    app.setStyleSheet(style);

    // specify some gl-options
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    //    test();

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


void test()
{
    /**
     * Minimal example of the implemented raytracing pipeline.
     * Initially used as minimal test environment, this funcion
     * shows the general use of the Simulation engine
     */

    QVector3D origin(0.0f,0.0f,0.0f);
    QVector3D target(1.0f,0.0f,0.0f);

    QVector3D direction = target - origin;

    qDebug() << "Running test:";
    qDebug() << direction;

    Sphere mirror(1.0, {2.0,-0.5,0.0});
    Sphere sphere(1.0, {1.0, 2.0, 0.0});

    Ray r(origin, direction.normalized());
    std::pair<Hitpoint, Hitpoint> hpp;
    if(mirror.intersect(r,&hpp)) {
        qDebug() << "  > Mirror first hitpoint" << hpp.first.position << "normal" << hpp.first.normal;
        qDebug() << "  > Mirror second hitpoint" << hpp.second.position << "normal" << hpp.second.normal;

        QVector3D ref = r.reflect(hpp.first.normal);
        qDebug() << "  >" << "Reflected direction:" << ref;


        std::pair<Hitpoint, Hitpoint> hpp2;
        Ray r2(hpp.first.position, ref);
        if(sphere.intersect(r2, &hpp2)) {
            qDebug().nospace() << Q_FUNC_INFO << " :" << __LINE__;
            qDebug() << "  > Sphere first hitpoint" << hpp2.first.position << "normal" << hpp2.first.normal;
            qDebug() << "  > Sphere second hitpoint" << hpp2.second.position << "normal" << hpp2.second.normal;

            QVector3D ref2 = r2.reflect(hpp2.first.normal);
            qDebug() << "  >" << "Reflected direction:" << ref2;

        }
    }
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
