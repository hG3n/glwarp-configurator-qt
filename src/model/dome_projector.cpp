#include <string>
#include <fstream>
#include <sstream>

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

#include "ray.hpp"
#include "dome_projector.hpp"

DomeProjector::DomeProjector(ProjectorFrustum *frustum,
                             int grid_rings,
                             int grid_ring_elements,
                             QVector3D const &position,
                             int dome_rings,
                             int dome_ring_elements)
    : _frustum(frustum)
    , _position(position)
    , _grid_rings(grid_rings)
    , _grid_ring_elements(grid_ring_elements)
    , _dome_rings(dome_rings)
    , _dome_ring_elements(dome_ring_elements)
{
    initialize();
}

DomeProjector::~DomeProjector()
{
    delete _frustum;
}

/*
 * methods
 */
void DomeProjector::initialize()
{
    generateRadialGrid();
    _frustum->translate(_position);

    QMatrix4x4 translation;
    translation.translate(_position);
    for (auto &point: sample_grid)
        point = translation * point;

    generateDomeVertices();

    // scale dome vertices to match the given size
    QMatrix4x4 scale_mat;
    scale_mat.scale( QVector3D(1.6f, 1.6f, 1.6f) );
    for (auto &vert: dome_vertices)
        vert = scale_mat * vert;
}

void DomeProjector::calculateTransformationMesh()
{
    // create green list here
    std::map<int, int> map;

    float last_distance = std::numeric_limits<float>::max();
    int last_hitpoint_idx = 0;

    // todo add binary search here
    for (unsigned int vert_idx = 0; vert_idx < dome_vertices.size(); ++vert_idx)
    {
        for (unsigned int hp_idx = 0; hp_idx < second_hits.size(); ++hp_idx)
        {
            float current_distance = (second_hits[hp_idx] - dome_vertices[vert_idx]).length();
            if (current_distance < last_distance)
            {
                last_distance = current_distance;
                last_hitpoint_idx = static_cast<int>(hp_idx);
            }
        }
        last_distance = std::numeric_limits<float>::max();
        map.insert(std::pair<int, int>(vert_idx, last_hitpoint_idx));
    }

    // calculate mapping
    std::vector<QVector3D> screen_points;
    std::vector<QVector3D> texture_points;
    for (auto pair : map)
    {
        texture_points.push_back(this->dome_vertices[pair.first]);
        screen_points.push_back(this->sample_grid[pair.second]);
    }

    std::vector<QVector3D> screen_points_adjusted;
    for (auto point : screen_points)
        screen_points_adjusted.push_back(QVector3D(point.x(), point.y(), 0.0f));

    // normalize texture points
    float texture_min_x = findMinValues(texture_points).x();
    float texture_max_x = findMaxValues(texture_points).x();
    float texture_min_z = findMinValues(texture_points).z();
    float texture_max_z = findMaxValues(texture_points).z();

    std::vector<QVector3D> texture_coords_normalized;
    for (auto point:texture_points)
    {
        float new_x = mapToRange(point.x(), texture_min_x, texture_max_x, 0.0f, 1.0f);
        float new_y = mapToRange(point.z(), texture_min_z, texture_max_z, 0.0f, 1.0f);
        texture_coords_normalized.emplace_back(QVector3D(new_x, new_y, 0.0f));
    }

    this->mesh_coords = screen_points_adjusted;
    this->texture_coords = texture_coords_normalized;
}

void DomeProjector::calculateDomeHitpoints(Sphere *mirror, Sphere *dome)
{
    first_hits.clear();
    second_hits.clear();

    // translate dome vertices to the domes position
    QMatrix4x4 dome_translation;
    dome_translation.translate(dome->get_position());
    for(auto &vertex : dome_vertices)
        vertex = dome_translation * vertex;

    // raycast for each samplepoint
    for (unsigned int i = 0; i < sample_grid.size(); ++i)
    {

        // calculate initial ray direction
        QVector3D initial_direction = sample_grid[i] - _position;

        // build ray and define hitpoint
        Ray r(_position, initial_direction.normalized());
        std::pair<Hitpoint, Hitpoint> hpp;
        if (mirror->intersect(r, &hpp))
        {

            first_hits.push_back(hpp.first.position);

            // reflect ray
            QVector3D ref = r.reflect(hpp.first.normal);

            Ray r2(hpp.first.position, ref.normalized());
            std::pair<Hitpoint, Hitpoint> hpp2;
            if (dome->intersect(r2, &hpp2)) {
                if (hpp2.second.position.y() > dome->get_position().y())
                    second_hits.push_back(hpp2.second.position);
                else
                    second_hits.push_back({1000, 1000, 1000});
            }
            else {
                second_hits.push_back({1000, 1000, 1000});
            }

        }
        else {
            first_hits.push_back({1000, 1000, 1000});
        }

    }

}

void DomeProjector::updateFromConfig(ModelConfig *config)
{
    // translate reset frustum position & update fov
    QVector3D current_frustum_pos = _frustum->getPosition();
    _frustum->translate(-current_frustum_pos);
    _frustum->setFOV(config->dome_projector.fov);

    // update sample grid
    _grid_rings = config->dome_projector.num_grid_rings;
    _grid_ring_elements = config->dome_projector.num_grid_ring_elements;
    generateRadialGrid();

    // translate frustum back & apply new transformation
    _frustum->translate(current_frustum_pos);
    translate(config->dome_projector.position);

    // move sample grid to current near clipping plane
    QMatrix4x4 translation;
    translation.translate(_position);
    for (auto &point: sample_grid)
        point = translation * point;

    // update dome geometry
    _dome_rings =  config->dome_projector.num_mesh_rings;
    _dome_ring_elements = config->dome_projector.num_mesh_ring_elements;
    generateDomeVertices();

    // scale dome vertices to match the given size
    QMatrix4x4 scale_mat;
    scale_mat.scale( QVector3D(1.6f, 1.6f, 1.6f) );
    for (auto &vert: dome_vertices)
        vert = scale_mat * vert;
}

void DomeProjector::generateRadialGrid()
{
    // if there are elements in there clear the vector
    if (!sample_grid.empty())
        sample_grid.clear();

    auto near = _frustum->getNearCorners();

    float step_size = fabsf((near[ProjectorFrustum::TL].x() - near[ProjectorFrustum::TR].x()) / _grid_rings) / 2;

    // define center point
    float center_x = near[ProjectorFrustum::TR].x() + near[ProjectorFrustum::TL].x();
    float center_y = near[ProjectorFrustum::TR].y() + near[ProjectorFrustum::BL].y();
    float center_z = near[ProjectorFrustum::TL].z();
    QVector3D center_point(center_x, center_y, center_z);

    float step_angle = 360.0f / _grid_ring_elements;

    float bottom_y = near[ProjectorFrustum::BR].y();
    float top_y = near[ProjectorFrustum::TL].y();

    sample_grid.push_back(center_point);
    for (int ring_idx = 1; ring_idx < _grid_rings + 1; ++ring_idx)
    {
        for (int ring_point_idx = 0; ring_point_idx < _grid_ring_elements; ++ring_point_idx)
        {
            QQuaternion euler_quat = QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f,(step_angle * ring_point_idx));
            QVector3D coord = euler_quat * QVector3D(ring_idx * step_size, 0.0f, 0.0f);

            if (coord.y() < top_y && coord.y() > bottom_y)
                // push back the rotated point at the near clipping planes z position
                sample_grid.emplace_back(QVector3D(coord.x(), coord.y(), near[ProjectorFrustum::TL].z()));
        }
    }

}

void DomeProjector::generateDomeVertices()
{
    if(!dome_vertices.empty())
        dome_vertices.clear();

    /*
     * Remember remember ...
     * THETA - AROUND Y
     * PHI - X AND
     */
    float delta_theta = 360.0f / (float) (this->_dome_ring_elements);
    float delta_phi = 90.0f / (float) (this->_dome_rings);

    // define center point
    std::vector<QVector3D> vertices;
    QVector3D pole_cap(0.0f, 1.0f, 0.0f);
    vertices.push_back(pole_cap);

    for (int ring_idx = 1; ring_idx < this->_dome_rings + 1; ++ring_idx)
    {
        QQuaternion phi_quat = QQuaternion::fromAxisAndAngle(0.0f,0.0f,1.0f,(ring_idx * delta_phi));
        QVector3D vec = phi_quat * QVector3D(pole_cap.x(), pole_cap.y(), pole_cap.z());
        for (int segment_idx = 0; segment_idx < this->_dome_ring_elements; ++segment_idx)
        {
            QQuaternion theta_quat = QQuaternion::fromAxisAndAngle(0.0f,1.0f,0.0f,(segment_idx * delta_theta));
            QVector3D final = theta_quat * vec;
            vertices.push_back(final);
        }
    }

    this->dome_vertices = vertices;
}

/*
 * Getter
 */
ProjectorFrustum *DomeProjector::getFrustum() const
{
    return _frustum;
}

void DomeProjector::translate(QVector3D position)
{
    QVector3D diff = position - _position;

    QMatrix4x4 translation;
    translation.translate(diff);

    _position = translation * _position;
    _frustum->translate(diff);
}

std::vector<QVector3D> DomeProjector::getMeshCoords() const
{
    std::vector<QVector3D> mesh_copy(mesh_coords);
    QVector3D meta(_dome_rings, _dome_ring_elements, mesh_coords.size());
    mesh_copy.push_back(meta);
    return mesh_copy;
}

std::vector<QVector3D> DomeProjector::getTexCoords() const
{
    std::vector<QVector3D> tex_copy(texture_coords);
    QVector3D meta(_dome_rings, _dome_ring_elements, texture_coords.size());
    tex_copy.push_back(meta);
    return tex_copy;
}


QVector3D DomeProjector::findMinValues(std::vector<QVector3D> vector)
{
    float smallest_x = std::numeric_limits<float>::max();
    float smallest_y = smallest_x;
    float smallest_z = smallest_x;

    for (auto point: vector) {
        if (point.x() < smallest_x)
            smallest_x = point.x();

        if (point.y() < smallest_y)
            smallest_y = point.y();

        if (point.z() < smallest_z)
            smallest_z = point.z();
    }

    return QVector3D(smallest_x, smallest_y, smallest_z);
}


QVector3D  DomeProjector::findMaxValues(std::vector<QVector3D> vector)
{
    float smallest_x = std::numeric_limits<float>::min();
    float smallest_y = smallest_x;
    float smallest_z = smallest_x;

    for (auto point: vector) {
        if (point.x() > smallest_x)
            smallest_x = point.x();

        if (point.y() > smallest_y)
            smallest_y = point.y();

        if (point.z() > smallest_z)
            smallest_z = point.z();
    }

    return QVector3D(smallest_x, smallest_y, smallest_z);
}


float DomeProjector::mapToRange(float value, float in_min, float in_max, float out_min, float out_max)
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
