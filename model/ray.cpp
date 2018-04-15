//
// Created by Hagen Hiller on 18/12/17.
//

#include <ostream>
#include <string>

#include "ray.hpp"

/**
 * default c'tor
 */
Ray::Ray()
        : origin(QVector3D())
        , direction(QVector3D()) {}

/**
 * c'tor
 * @param origin
 */
Ray::Ray(const QVector3D &origin)
        : origin(origin)
        , direction(QVector3D()) {}

/**
 * c'tor
 * @param origin
 * @param direction
 */
Ray::Ray(const QVector3D &origin, const QVector3D &direction)
        : origin(origin)
        , direction(direction) {}


/**
 * copy c'tor
 * @param ray
 */
Ray::Ray(const Ray &ray)
        : origin(ray.origin)
        , direction(ray.direction) {}

/**
 * d'tor
 */
Ray::~Ray() {}


/**
 * reflect ray along given normal
 * @param normal
 * @return
 */
QVector3D Ray::reflect(QVector3D const &normal) const {
//    return this->direction - 2.0f * (glm::dot(this->direction, glm::normalize(normal))) * glm::normalize(normal);
    return direction - 2.0f * (QVector3D::dotProduct(direction, normal.normalized()) * normal.normalized());
}

/**
 * stream << output operator
 * @param os
 * @param ray
 * @return
 */
std::ostream &operator<<(std::ostream &os, const Ray &ray) {
//    std::string origin_s = ::vecstr(ray.origin);
//    std::string direction_s = utility::vecstr(ray.direction);
    os << "origin: " << ray.origin << " direction: " << ray.direction;
    return os;
}
