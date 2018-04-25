
// Created by Hagen Hiller on 18/12/17.
//

#ifndef RAYCAST_SPHERE_H
#define RAYCAST_SPHERE_H

#include <utility>
#include <ostream>

#include "ray.hpp"
#include "hitpoint.hpp"

class Sphere {

    public:
        // c'tor
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

        float get_radius() const;
        QVector3D get_position() const;

        void set_position(const QVector3D &new_position);
        void set_radius(float new_radius);

        /**
         * overloaded out stream operator
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
