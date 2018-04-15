//
// Created by everest on 14.03.18.
//

#ifndef RAYCAST_PROJECTOR_FRUSTUM_H
#define RAYCAST_PROJECTOR_FRUSTUM_H

#include <iostream>
#include <string>
#include <map>

#include <QVector3D>

class ProjectorFrustum {

    public:

        enum ClippingPlane {
            NEAR, FAR
        };

        enum Corner {
            TL, TR, BL, BR
        };

        /**
         * Default c'tor
         */
        ProjectorFrustum();

        /**
         * Custom c'tor
         * creates new frustum in
         * @param _aspect_ratio
         * @param _fov
         * @param _near
         * @param _far
         */
        ProjectorFrustum(float _aspect_ratio, float _fov, float _near, float _far);

        /**
         * Translate to current position
         * @param position
         */
        void translateTo(QVector3D const &position);

        /**
         * Rotate by given degree angle around specified axis
         * @param angle
         * @param axis
         */
        void rotate(float angle, QVector3D const &axis);


        /**
         * @brief setFOV
         * @param fov
         */
        void setFOV(float fov);

        /**
         * Returns near clipping plane corners
         * @return
         */
        std::map<Corner, QVector3D> const& getNearCorners() const;

        /**
         * Returns far clipping plane corners
         * @return
         */
        std::map<Corner, QVector3D> const& getFarCorners() const;


        /**
         * returns frustum eye ray
         * @return
         */
        QVector3D const& getEye() const;

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
