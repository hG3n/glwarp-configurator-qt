#include <iostream>

#include <qmath.h>
#include <QVector3D>
#include <QMatrix4x4>

#include "sphere.hpp"

#define epsilon 0.000001f

// -----------------------------------------------------------------------------------------
// C'TORs

/**
 * default c'tor
 */
Sphere::Sphere()
        : _radius(1.0f), _center(QVector3D()) {}


/**
 * c'tor
 * @param radius
 */
Sphere::Sphere(float radius)
        : _radius(radius)
        , _center(QVector3D()) {}


/**
 * c'tor
 * @param radius
 * @param position
 */
Sphere::Sphere(float radius, QVector3D position)
        : _radius(radius)
        , _center(position) {}


/**
 * d'tor
 */
//Sphere::~Sphere() = default;

// -----------------------------------------------------------------------------------------
// METHODS

/**
 * Validates, whether the given ray intersects the Sphere and retunts a pair of Hitpoints.
 * @param r
 * @param hp_pair
 * @return bool success
 */
bool Sphere::intersect(Ray const &r, std::pair<Hitpoint, Hitpoint> *hp_pair) {

//    qDebug() << "Ray - origin: " <<  r.origin << " direction: " << r.direction;
    bool verbose = false;

    // vector from current ray orign to sphere center
//    QVector3D L = _center - r.origin;
    QVector3D L = r.origin - _center;

    // distance from ray origin to intersection with the spheres perpendicular vector
    double t_ca = QVector3D::dotProduct(L, r.direction);

    // if t_ca is smaller than 0 the ray might be pointing in the wrong direction
    if (t_ca < 0) {
        if(verbose)
            qDebug() << "t_ca < 0";
        return false;
    }

    // calc vector length perpendicular to the ray through the spheres center
    double d = sqrt(QVector3D::dotProduct(L,L) - pow(t_ca, 2));
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
    QVector3D temp = _center - r.origin;// - _center;

    // first hitpoint
    QVector3D P1 = r.origin + ( t_0 * r.direction);
//    QVector3D nom1 = (temp + t_0 * r.direction);
//    QVector3D N1 = nom1.normalized()/ _radius;
    QVector3D N1 = (P1 - _center).normalized();

    hp_pair->first.position = P1;
    hp_pair->first.normal = N1;

    // second hitpoint
    QVector3D P2 = r.origin + ((float) t_1 * r.direction);
    QVector3D nom2 = temp + (float)t_1 * r.direction;
    QVector3D N2 = nom2.normalized() / this->_radius;
    QVector3D N22 = (P2 - _center).normalized();

//    qDebug() << N2;
//    qDebug() << N22;
//    qDebug() << "";

    hp_pair->second.position = P2;
    hp_pair->second.normal = N2;

    return true;
}



// -----------------------------------------------------------------------------------------
// GETTER

/**
 * get radius
 * @return
 */
float Sphere::get_radius() const {
    return _radius;
}

/**
 * get position
 * @return
 */
QVector3D Sphere::get_position() const {
    return _center;
}

// -----------------------------------------------------------------------------------------
// SETTER

/**
 * set position
 * @param new_position
 */
void Sphere::set_position(QVector3D new_position) {
    _center = new_position;
}


/**
 * set radius
 * @param new_radius
 */
void Sphere::set_radius(float new_radius) {
    _radius = new_radius;
}

// -----------------------------------------------------------------------------------------
// OPERATORS

/**
 * overloaded out stream operator
 * @param os
 * @param sphere
 * @return
 */
std::ostream &operator<<(std::ostream &os, const Sphere &sphere) {
    os << "<Sphere | radius: " << sphere._radius
       << " | center: ["
       << sphere._center.x() << ","
       << sphere._center.y() << ","
       << sphere._center.z() << "]"
       << ">";
    return os;
}
