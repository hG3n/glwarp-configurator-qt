//
// Created by Hagen Hiller on 18/12/17.
//

#ifndef RAYCAST_RAY_H
#define RAYCAST_RAY_H

#include <QVector3D>

#include <ostream>

struct Ray {

    Ray();
    Ray(const QVector3D &origin);
    Ray(const QVector3D &origin, const QVector3D &direction);
    Ray(const Ray& ray);
    ~Ray();


    QVector3D reflect(QVector3D const& normal) const;
    friend std::ostream &operator<<(std::ostream &os, const Ray &ray);

    // members
    QVector3D origin;
    QVector3D direction;
};

#endif //RAYCAST_RAY_H
