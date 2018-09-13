#ifndef RAYCAST_RAY_H
#define RAYCAST_RAY_H

#include <QVector3D>

#include <ostream>

/**
 * @brief The Ray struct describes a ray that can be cast from an origin towards a specified direction.
 */
struct Ray {

    Ray();
    Ray(const QVector3D &origin);
    Ray(const QVector3D &origin, const QVector3D &direction);
    Ray(const Ray& ray);
    ~Ray();


    /**
     * @brief Returns the reflection vector of this ray with a specified normal.
     * @param normal
     * @return
     */
    QVector3D reflect(QVector3D const& normal) const;

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
         * @param ray
         * @return
         */
    friend std::ostream &operator<<(std::ostream &os, const Ray &ray);

    // members
    QVector3D origin;
    QVector3D direction;
};

#endif //RAYCAST_RAY_H
