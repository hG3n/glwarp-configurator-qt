#ifndef RAYCAST_SPHERE_H
#define RAYCAST_SPHERE_H

#include <utility>
#include <ostream>

#include "ray.hpp"
#include "hitpoint.hpp"

class Sphere {

    public:
        /**
         * @brief Sphere c'tor
         */
        Sphere();

        /**
         * @brief Sphere
         */
        Sphere(float radius);

        /**
         * @brief Sphere
         * @param radius
         * @param position
         */
        Sphere(float radius, const QVector3D &position);

        /**
         * Validates, whether the given ray intersects the Sphere and retunts a pair of Hitpoints.
         * @param r
         * @param hp_pair
         * @return bool success
         */
        bool intersect(Ray const &r, std::pair<Hitpoint, Hitpoint> *hp_pair);

        /**
         * @brief Returns the spheres radius.
         * @return
         */
        float get_radius() const;

        /**
         * @brief Returns the spheres position.
         * @return
         */
        QVector3D get_position() const;

        /**
         * @brief Set the Spheres position.
         * @param new_position
         */
        void set_position(const QVector3D &new_position);

        /**
         * @brief Set the Spheres radius.
         * @param new_radius
         */
        void set_radius(float new_radius);

        /**
         * @brief operator << used for printing the Class using std<<cout;
         *
         * Because of reasons, Qt has its own way of printing things.
         * Therefor this'll not work with Qt's qDebug() <<
         *
         * I'm not going to look into this,
         * ... just because.
         *
         * @param os
         * @param sphere
         * @return
         */
        friend std::ostream &operator<<(std::ostream &os, const Sphere &sphere);

    private:
        float _radius;
        QVector3D _center;
};

#endif //RAYCAST_SPHERE_H
