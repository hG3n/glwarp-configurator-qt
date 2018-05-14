#include <iostream>

#include <qmath.h>
#include <QVector3D>
#include <QMatrix4x4>

#include "sphere.hpp"

#define epsilon 0.000001f

// -----------------------------------------------------------------------------------------
// C'TORs

Sphere::Sphere()
        : _radius(1.0f)
        , _center(QVector3D()) {}

Sphere::Sphere(float radius)
        : _radius(radius)
        , _center(QVector3D()) {}

Sphere::Sphere(float radius, const QVector3D &position)
        : _radius(radius)
        , _center(position) {}

//Sphere::~Sphere() = default;


bool Sphere::intersect(Ray const &r, std::pair<Hitpoint, Hitpoint> *hp_pair) {

    bool verbose = false;

    // vector from current ray orign to sphere center
    QVector3D L = _center - r.origin;

    // distance from ray origin to intersection with the spheres perpendicular vector
    double t_ca = QVector3D::dotProduct(L, r.direction);

    // if t_ca is smaller than 0 the ray might be pointing in the wrong direction
    if (t_ca < 0) {
        if(verbose)
            qDebug() << "t_ca < 0: ray might pint in the wrong direction!";
        return false;
    }

    // calc vector length perpendicular to the ray through the spheres center
    double d = sqrt(QVector3D::dotProduct(L,L) - pow(t_ca, 2));
    if(verbose)
        qDebug() << "  >" << "d" << d;
    if (d < 0 || d > _radius) {

        if(verbose) {
            if (d < 0)
                qDebug() << "d < 0";

            if (d > _radius)
                qDebug() << "d > _radius";
        }

        return false;
    }

    // calculate the distance from the first hitpoint to d
    double t_hc = sqrt(pow(_radius, 2) - pow(d, 2));

    double t_0 = t_ca - t_hc;
    double t_1 = t_ca + t_hc;

    // help vector
    QVector3D temp =  r.origin - _center;

    // first hitpoint
    QVector3D P1 = r.origin + ( t_0 * r.direction);
    QVector3D nom1 = (temp + t_0 * r.direction);
    QVector3D N1 = nom1.normalized() / _radius;

    hp_pair->first.position = P1;
    hp_pair->first.normal = N1;

    // second hitpoint
    QVector3D P2 = r.origin + ((float) t_1 * r.direction);
    QVector3D nom2 = temp + (float)t_1 * r.direction;
    QVector3D N2 = nom2.normalized() / _radius;

    hp_pair->second.position = P2;
    hp_pair->second.normal = N2;

    return true;
}

// -----------------------------------------------------------------------------------------
// GETTER
float Sphere::get_radius() const {
    return _radius;
}

QVector3D Sphere::get_position() const {
    return _center;
}

// -----------------------------------------------------------------------------------------
// SETTER
void Sphere::set_position(const QVector3D &new_position) {
    _center = new_position;
}

void Sphere::set_radius(float new_radius) {
    _radius = new_radius;
}

// -----------------------------------------------------------------------------------------
// OPERATORS
std::ostream &operator<<(std::ostream &os, const Sphere &sphere) {
    os << "<Sphere | radius: " << sphere._radius
       << " | center: [" << sphere._center.x() << "," << sphere._center.y() << "," << sphere._center.z() << "]"
       << ">";
    return os;
}
