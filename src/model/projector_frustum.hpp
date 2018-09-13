//
// Created by everest on 14.03.18.
//

#ifndef RAYCAST_PROJECTOR_FRUSTUM_H
#define RAYCAST_PROJECTOR_FRUSTUM_H

#include <iostream>
#include <string>
#include <map>

#include <QVector3D>

/**
 * @brief The ProjectorFrustum class describes a light projector frustum
 */
class ProjectorFrustum {

    public:

        enum ClippingPlane {
            NEAR, FAR
        };

        enum Corner {
            TL, TR, BL, BR
        };

        /**
         * @brief ProjectorFrustum
         */
        ProjectorFrustum();

        /**
         * @brief Creates a new frustum with the given values.
         * @param _aspect_ratio
         * @param _fov
         * @param _near
         * @param _far
         */
        ProjectorFrustum(float _aspect_ratio, float _fov, float _near, float _far);

        /**
         * @brief Translate to current position.
         * @param position
         */
        void translate(QVector3D const &position);

        /**
         * @brief Rotate by given degree angle around specified axis.
         * @param angle
         * @param axis
         */
        void rotate(float angle, QVector3D const &axis);

        /**
         * @brief Set the frustum field of view.
         * @param fov
         */
        void setFOV(float fov);

        /**
         * @brief Returns near clipping plane corners.
         * @return
         */
        std::map<Corner, QVector3D> const& getNearCorners() const;

        /**
         * @brief Returns far clipping plane corners.
         * @return
         */
        std::map<Corner, QVector3D> const& getFarCorners() const;

        /**
         * @brief Returns frustum eye ray.
         * @return
         */
        QVector3D const& getEye() const;

        /**
         * @brief Returns the position.
         * @return
         */
        QVector3D const& getPosition() const;


    private:
        void initialize();

    private:
        float _aspect_ratio;
        float _fov;
        float _near;
        float _far;

        QVector3D _eye;
        QVector3D _position;
        QVector3D _rotation;

        std::map<Corner, QVector3D> _near_corners;
        std::map<Corner, QVector3D> _far_corners;
};

#endif //RAYCAST_PROJECTOR_FRUSTUM_H
