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

std::ostream &operator<<(std::ostream &os, const Hitpoint &hitpoint) {
//    std::string pos_string = utility::vecstr(hitpoint.position);
//    std::string norm_string = utility::vecstr(hitpoint.normal);
    os << "position: " << hitpoint.position << " normal: " << hitpoint.normal << " t: " << hitpoint.t;
    return os;
}




