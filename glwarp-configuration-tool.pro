#-------------------------------------------------
#
# Project created by QtCreator 2018-03-26T20:48:33
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = glwarp-configuration-tool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        ui/mainwindow.cpp \
        ui/glwidget.cpp \
        ui/mainwidget.cpp \
        ui/propertyedit.cpp \
        ui/configurationwidget.cpp \
        ui/projectorconfigwidget.cpp \
        ui/vector3edit.cpp \
        model/dome_projector.cpp\
        model/file_writer.cpp\
        model/hitpoint.cpp\
        model/path.cpp\
        model/projector_frustum.cpp\
        model/ray.cpp\
        model/sphere.cpp\
        model/simulation.cpp \
        lib/json11.cpp \
        config.cpp \

HEADERS  += ui/mainwindow.h \
            ui/glwidget.h \
            ui/mainwidget.h \
            ui/propertyedit.hpp \
            ui/configurationwidget.hpp \
            ui/projectorconfigwidget.hpp \
            ui/vector3edit.hpp \
            model/dome_projector.hpp\
            model/file_writer.hpp\
            model/hitpoint.hpp\
            model/path.hpp\
            model/projector_frustum.hpp\
            model/ray.hpp\
            model/sphere.hpp\
            model/simulation.h \
            lib/json11.hpp \
            config.h \

DISTFILES +=  shaders/simple.frag \
              shaders/simple.vert \
    _RES/style.css


RESOURCES +=  _RES/resources.qrc
