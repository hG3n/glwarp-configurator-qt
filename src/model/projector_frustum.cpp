#include <QVector3D> #include <QMatrix4x4>
#include <QQuaternion>
#include <QDebug>

#include <QMatrix4x4>

#include "projector_frustum.hpp"

ProjectorFrustum::ProjectorFrustum()
    : _aspect_ratio(1)
    , _fov(90.0f)
    , _near(1.0f)
    , _far(2.0f)
    , _eye(QVector3D(0.0f, 0.0, -1.0f))
    , _position(QVector3D())
    , _rotation(QVector3D())
{}

ProjectorFrustum::ProjectorFrustum(float _aspect_ratio, float _fov, float _near, float _far)
    : _aspect_ratio(_aspect_ratio)
    , _fov(_fov)
    , _near(_near)
    , _far(_far)
    , _eye(QVector3D(0.0f, 0.0, -1.0f))
    , _position(QVector3D())
    , _rotation(QVector3D())
{
    initialize();
}

void ProjectorFrustum::initialize()
{
    // create foward vector
    _eye.setZ(-_near);

    // calculate top point
    QQuaternion euler = QQuaternion::fromAxisAndAngle(1.0f,0.0f,0.0f, (_fov /2.0f ));
    QVector3D near_top_point = euler * _eye;

    float width = near_top_point.y() * 2.0f;
    QVector3D near_tl(-(width * _aspect_ratio / 2.0f), near_top_point.y(), near_top_point.z());
    QVector3D near_bl(-(width * _aspect_ratio / 2.0f), -near_top_point.y(), near_top_point.z());
    QVector3D near_tr(width * _aspect_ratio / 2.0f, near_top_point.y(), near_top_point.z());
    QVector3D near_br(width * _aspect_ratio / 2.0f, -near_top_point.y(), near_top_point.z());

    _near_corners[TL] = near_tl;
    _near_corners[TR] = near_tr;
    _near_corners[BR] = near_br;
    _near_corners[BL] = near_bl;

    _eye.setZ(-_far);

    // calculate top point
    QVector3D far_top_point = euler * _eye;

    width = far_top_point.y() * 2.0f;
    QVector3D far_tl(-(width * _aspect_ratio / 2.0f), far_top_point.y(), far_top_point.z());
    QVector3D far_bl(-(width * _aspect_ratio / 2.0f), -far_top_point.y(), far_top_point.z());
    QVector3D far_tr(width * _aspect_ratio / 2.0f, far_top_point.y(), far_top_point.z());
    QVector3D far_br(width * _aspect_ratio / 2.0f, -far_top_point.y(), far_top_point.z());

    _far_corners[TL] = far_tl;
    _far_corners[TR] = far_tr;
    _far_corners[BR] = far_br;
    _far_corners[BL] = far_bl;
}


void ProjectorFrustum::translate(QVector3D const &position)
{
    QMatrix4x4 translation;
    translation.translate(position);
    _position = translation * _position;
    _eye = translation * _eye;

    // translate each near corner
    for (auto &corner: _near_corners)
        corner.second = translation * corner.second;

    for (auto &corner: _far_corners)
        corner.second = translation * corner.second;
}


void ProjectorFrustum::rotate(float angle, QVector3D const &axis)
{
    // make rotation matrix
    QQuaternion rotation = QQuaternion::fromAxisAndAngle(axis, angle);
    _eye = rotation * _eye;

    for (auto &corner: _near_corners)
        corner.second = rotation * corner.second;

    for (auto &corner: _far_corners)
        corner.second = rotation * corner.second;
}


void ProjectorFrustum::setFOV(float fov) {
    _fov = fov;
    initialize();
}


std::map<ProjectorFrustum::Corner, QVector3D> const &ProjectorFrustum::getNearCorners() const {
    return _near_corners;
}

std::map<ProjectorFrustum::Corner, QVector3D> const &ProjectorFrustum::getFarCorners() const {
    return _far_corners;
}

QVector3D const &ProjectorFrustum::getEye() const {
    return _eye;
}

QVector3D const &ProjectorFrustum::getPosition() const {
    return _position;
}
