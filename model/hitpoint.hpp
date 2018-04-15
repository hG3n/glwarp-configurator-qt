//
// Created by Hagen Hiller on 18/12/17.
//

#ifndef RAYCAST_HITPOINT_HPP
#define RAYCAST_HITPOINT_HPP

#include <ostream>

class Hitpoint {

    public:

        /**
         * @brief Hitpoint
         */
        Hitpoint();

        /**
         * @brief Hitpoint
         * @param position
         */
        Hitpoint(const QVector3D &position);

        /**
         * @brief Hitpoint
         * @param position
         * @param normal
         * @param t
         */
        Hitpoint(const QVector3D &position, const QVector3D &normal, double t);

        /**
         * @brief operator <<
         * @param os
         * @param hitpoint
         * @return
         */
        friend std::ostream &operator<<(std::ostream &os, const Hitpoint &hitpoint);

        // member
        QVector3D position;
        QVector3D normal;
        double t;
};


#endif //RAYCAST_HITPOINT_HPP

