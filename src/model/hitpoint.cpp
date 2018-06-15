#include <QVector3D>

#include "hitpoint.hpp"
Hitpoint::Hitpoint()
        : position(QVector3D())
        , normal(QVector3D())
        , t(0.0) {}


Hitpoint::Hitpoint(const QVector3D &position)
        : position(position)
        , normal(QVector3D())
        , t(0.0) {}

Hitpoint::Hitpoint(const QVector3D &position, const QVector3D &normal, double t)
        : position(position)
        , normal(normal)
        , t(t) {}
