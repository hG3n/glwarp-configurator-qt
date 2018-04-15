
// Created by Hagen Hiller on 18/12/17.
//

#ifndef RAYCAST_SPHERE_H
#define RAYCAST_SPHERE_H

#include <ostream>

#include "ray.hpp"
#include "hitpoint.hpp"

class Sphere {

    public:

        // c'tor
        Sphere();
        Sphere(float);
        Sphere(float, QVector3D);
        //    ~Sphere();

        // methods
        bool intersect(Ray const &r, std::pair<Hitpoint, Hitpoint> *hp_pair);

        // getter
        float get_radius() const;
        QVector3D get_position() const;

        // setter
        void set_radius(float);
        void set_position(QVector3D);

        // ostream
        friend std::ostream &operator<<(std::ostream &os, const Sphere &sphere);

    private:
        float _radius;
        QVector3D _center;
};

#endif //RAYCAST_SPHERE_H
