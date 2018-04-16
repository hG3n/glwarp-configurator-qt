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

DomeProjector::~DomeProjector()  {
    delete _frustum;
}


/*
 * methods
 */
void DomeProjector::initialize() {
    this->generateRadialGrid();
    this->generateDomeVertices();

    // scale dome vertices to match the given size
    QMatrix4x4 scale_mat;
    scale_mat.scale( QVector3D(1.6f, 1.6f, 1.6f) );
    for (auto &vert: _dome_vertices) {
        vert = scale_mat * vert;
    }

    // translate frustum
    _frustum->translateTo(_position);

    QMatrix4x4 translation;
    translation.translate(_position);
    for (auto &point: _sample_grid) {
        point = translation * point;
    }
}

std::vector<QVector3D> DomeProjector::calculateTransformationMesh() {
    // create green list here
    std::map<int, int> map;

    float last_distance = std::numeric_limits<float>::max();
    int last_hitpoint_idx = 0;

    for (unsigned int vert_idx = 0; vert_idx < this->_dome_vertices.size(); ++vert_idx) {
        for (unsigned int hp_idx = 0; hp_idx < this->_second_hits.size(); ++hp_idx) {
            float current_distance = (this->_second_hits[hp_idx] - this->_dome_vertices[vert_idx]).length();
            if (current_distance < last_distance) {
                last_distance = current_distance;
                last_hitpoint_idx = hp_idx;
            }
        }
        last_distance = std::numeric_limits<float>::max();
        map.insert(std::pair<int, int>(vert_idx, last_hitpoint_idx));
    }

    // calculate mapping
    std::vector<QVector3D> screen_points;
    std::vector<QVector3D> texture_points;
    for (auto pair : map) {
        texture_points.push_back(this->_dome_vertices[pair.first]);
        screen_points.push_back(this->_sample_grid[pair.second]);
    }

    // normalize screen list
    float screen_min_x = findMinValues(screen_points).x();
    float screen_max_x = findMaxValues(screen_points).x();
    float screen_min_y = findMinValues(screen_points).y();
    float screen_max_y = findMaxValues(screen_points).y();

    std::vector<QVector3D> screen_points_normalized;
    for (auto point : screen_points) {
        float new_x = mapToRange(point.x(), screen_min_x, screen_max_x, -1.0f, 1.0f);
        float new_y = mapToRange(point.y(), screen_min_y, screen_max_y, -1.0f, 1.0f);

        screen_points_normalized.emplace_back(QVector3D(new_x, new_y, 0.0f));
    }

    // normalize texture points
    float texture_min_x = findMinValues(texture_points).x();
    float texture_max_x = findMaxValues(texture_points).x();
    float texture_min_z = findMinValues(texture_points).z();
    float texture_max_z = findMaxValues(texture_points).z();

    std::vector<QVector3D> texture_coords_normalized;
    for (auto point:texture_points) {
        float new_x = mapToRange(point.x(), texture_min_x, texture_max_x, 0.0f, 1.0f);
        float new_y = mapToRange(point.z(), texture_min_z, texture_max_z, 0.0f, 1.0f);
        texture_coords_normalized.emplace_back(QVector3D(new_x, new_y, 0.0f));
    }

    this->_screen_points = screen_points_normalized;
    this->_texture_coords = texture_coords_normalized;

    return std::vector<QVector3D>();
}

void DomeProjector::calculateDomeHitpoints(Sphere *mirror, Sphere *dome) {

    qDebug() << "calculateDomeHitpoints";
    qDebug() << "Projector Position: " << _position;
    qDebug() << "Dome position: " << dome->get_position();
    qDebug() << "Mirror position: " << mirror->get_position();

    _first_hits.clear();
    _second_hits.clear();

    // translate dome vertices to the domes position
    QMatrix4x4 dome_translation;
    dome_translation.translate(dome->get_position());
    for(auto &vertex : _dome_vertices) {
        vertex = dome_translation * vertex;
    }

    // raycast for each samplepoint
    for (unsigned int i = 0; i < _sample_grid.size(); ++i) {

        // calculate initial ray direction
        QVector3D initial_direction = _sample_grid[i] - _position;

        // build ray and define hitpoint
        Ray r(_position, initial_direction.normalized());
        std::pair<Hitpoint, Hitpoint> hpp;
        if (mirror->intersect(r, &hpp)) {
            _first_hits.push_back(hpp.first.position);

            // reflect ray
            QVector3D ref = r.reflect(hpp.first.normal);

            Ray r2(hpp.first.position, ref.normalized());
            std::pair<Hitpoint, Hitpoint> hpp2;
            if (dome->intersect(r2, &hpp2)) {
                qDebug() << "pls die!";
                if (hpp2.second.position.y() > dome->get_position().y()) {
                    _second_hits.push_back(hpp2.second.position);
                } else {
                    _second_hits.push_back({1000, 1000, 1000});
                }
            } else {
                _second_hits.push_back({1000, 1000, 1000});
            }

        } else {
            _second_hits.push_back({1000, 1000, 1000});
        }

    }

}

void DomeProjector::updateFromConfig(ModelConfig *model_config) {
    _frustum->setFOV(model_config->dome_projector.fov);

    _position = model_config->dome_projector.position;
    _grid_rings = model_config->dome_projector.num_mesh_rings;
    _grid_ring_elements = model_config->dome_projector.num_grid_ring_elements;

    _dome_rings =  model_config->dome_projector.num_mesh_rings;
    _dome_ring_elements = model_config->dome_projector.num_mesh_ring_elements;

    initialize();
}

void DomeProjector::generateRadialGrid() {

    // if there are elements in there clear the vector
    if (!_sample_grid.empty()) {
        _sample_grid.clear();
    }

    auto near = _frustum->getNearCorners();
    for(auto pair: near) {
        qDebug() << pair.first << " " << pair.second;
    }

    float step_size = fabsf((near[ProjectorFrustum::TL].x() - near[ProjectorFrustum::TR].x()) / _grid_rings) / 2;

    // define center point
    float center_x = near[ProjectorFrustum::TR].x() + near[ProjectorFrustum::TL].x();
    float center_y = near[ProjectorFrustum::TR].y() + near[ProjectorFrustum::BL].y();
    float center_z = near[ProjectorFrustum::TL].z();
    QVector3D center_point(center_x, center_y, center_z);

    float step_angle = 360.0f / _grid_ring_elements;

    float bottom_y = near[ProjectorFrustum::BR].y();
    float top_y = near[ProjectorFrustum::TL].y();

    _sample_grid.push_back(center_point);
    for (int ring_idx = 1; ring_idx < _grid_rings + 1; ++ring_idx) {
        for (int ring_point_idx = 0; ring_point_idx < _grid_ring_elements; ++ring_point_idx) {
            QQuaternion euler_quat = QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f,(step_angle * ring_point_idx));
            QVector3D coord = euler_quat * QVector3D(ring_idx * step_size, 0.0f, 0.0f);

            if (coord.y() < top_y && coord.y() > bottom_y) {
                // push back the rotated point at the near clipping planes z position
                _sample_grid.emplace_back(QVector3D(coord.x(), coord.y(), near[ProjectorFrustum::TL].z()));
            }

        }
    }

}

void DomeProjector::generateDomeVertices() {
    /*
     * THETA - AROUND Y
     * PHI - X AND
     */
    float delta_theta = 360.0f / (float) (this->_dome_ring_elements);
    float delta_phi = 90.0f / (float) (this->_dome_rings);

    // define center point
    std::vector<QVector3D> vertices;
    QVector3D pole_cap(0.0f, 1.0f, 0.0f);
    vertices.push_back(pole_cap);

    for (int ring_idx = 1; ring_idx < this->_dome_rings + 1; ++ring_idx) {
        QQuaternion phi_quat = QQuaternion::fromAxisAndAngle(0.0f,0.0f,1.0f,(ring_idx * delta_phi));
        //        glm::quat phi_quat(QVector3D(glm::radians(ring_idx * delta_phi), 0.0f, 0.0f));
        QVector3D vec = phi_quat * QVector3D(pole_cap.x(), pole_cap.y(), pole_cap.z());
        for (int segment_idx = 0; segment_idx < this->_dome_ring_elements; ++segment_idx) {
            QQuaternion theta_quat = QQuaternion::fromAxisAndAngle(0.0f,1.0f,0.0f,(segment_idx * delta_theta));
            //            glm::quat theta_quat(QVector3D(0.0f, glm::radians(segment_idx * delta_theta), 0.0f));
            QVector3D final = theta_quat * vec;
            vertices.push_back(final);
        }
    }

    this->_dome_vertices = vertices;
}

/*
 * Getter
 */
ProjectorFrustum *DomeProjector::getFrustum() const {
    return _frustum;
}

std::vector<QVector3D> const &DomeProjector::get_sample_grid() const {
    return this->_sample_grid;
}

std::vector<QVector3D> const &DomeProjector::get_first_hits() const {
    return this->_first_hits;
}

std::vector<QVector3D> const &DomeProjector::get_second_hits() const {
    return this->_second_hits;
}

std::vector<QVector3D> const &DomeProjector::get_dome_vertices() const {
    return this->_dome_vertices;
}

std::vector<QVector3D> const &DomeProjector::get_screen_points() const {
    return this->_screen_points;
}

std::vector<QVector3D> const &DomeProjector::get_texture_coords() const {
    return this->_texture_coords;
}


QVector3D DomeProjector::findMinValues(std::vector<QVector3D> vector) {

    float smallest_x = std::numeric_limits<float>::max();
    float smallest_y = smallest_x;
    float smallest_z = smallest_x;

    for (auto point: vector) {
        if (point.x() < smallest_x) {
            smallest_x = point.x();
        }

        if (point.y() < smallest_y) {
            smallest_y = point.y();
        }

        if (point.z() < smallest_z) {
            smallest_z = point.z();
        }
    }

    return QVector3D(smallest_x, smallest_y, smallest_z);

}


QVector3D  DomeProjector::findMaxValues(std::vector<QVector3D> vector) {

    float smallest_x = std::numeric_limits<float>::min();
    float smallest_y = smallest_x;
    float smallest_z = smallest_x;

    for (auto point: vector) {
        if (point.x() > smallest_x) {
            smallest_x = point.x();
        }

        if (point.y() > smallest_y) {
            smallest_y = point.y();
        }

        if (point.z() > smallest_z) {
            smallest_z = point.z();
        }
    }

    return QVector3D(smallest_x, smallest_y, smallest_z);

}


float DomeProjector::mapToRange(float value, float in_min, float in_max, float out_min, float out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*
 * ostream
 */
std::ostream &operator<<(std::ostream &os, const DomeProjector &projector) {
    os << "Dome Projector:" << "\n"
       << "  position: [" << projector._position.x() << " , " << projector._position.y() << " , "<< projector._position.z() << "]\n"
       << "  grid_rings: " << projector._grid_rings << "\n"
       << "  grid_ring_elements: " << projector._grid_ring_elements << "\n"
       << "  sample_grid: " << projector._sample_grid.size() << "\n"
       << "  first_hits: " << projector._first_hits.size() << "\n"
       << "  second_hits: " << projector._second_hits.size();
    return os;
}


