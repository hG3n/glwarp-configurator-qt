#include "simulation.h"

Simulation::Simulation()
    : _dp(nullptr)
    , _mirror(nullptr)
    , _dome(nullptr)
{}


Simulation::~Simulation()
{
    delete _dp;
    delete _mirror;
    delete _dome;
}


void Simulation::runCalculations()
{
    _dp->calculateDomeHitpoints(_mirror, _dome);
    _dp->calculateTransformationMesh();
}


void Simulation::initialize(ModelConfig *config)
{
    float aspect_ratio = (float)config->dome_projector.screen_width /  config->dome_projector.screen_height;

    ProjectorFrustum *new_frustum = new ProjectorFrustum(aspect_ratio,
                                                         config->dome_projector.fov,
                                                         1.0f, 1.5f);

    _dp = new DomeProjector(new_frustum,
                            config->dome_projector.num_grid_rings,
                            config->dome_projector.num_grid_ring_elements,
                            config->dome_projector.position,
                            config->dome_projector.num_mesh_rings,
                            config->dome_projector.num_mesh_ring_elements);

    _mirror = new Sphere(config->mirror.radius,
                         config->mirror.position);

    _dome = new Sphere(config->dome.radius,
                       config->dome.position);
}


void Simulation::updateFromConfig(ModelConfig *config)
{
    _dp->updateFromConfig(config);

    _mirror->set_radius(config->mirror.radius);
    _mirror->set_position(config->mirror.position);

    _dome->set_radius(config->dome.radius);
    _dome->set_position(config->dome.position);
}


Scene Simulation::getCurrentScene() const
{
    Scene s;
    s.dome_vertices = _dp->dome_vertices;
    s.first_hits = _dp->first_hits;
    s.second_hits = _dp->second_hits;
    s.sample_grid = _dp->sample_grid;
    s.screen_points = _dp->mesh_coords;
    s.texture_coords = _dp->texture_coords;
    s.far_corners = _dp->getFrustum()->getFarCorners();
    s.near_corners = _dp->getFrustum()->getNearCorners();
    return s;
}


std::vector<QVector3D> Simulation::getTransformationMesh() const
{
    return _dp->getMeshCoords();
}

std::vector<QVector3D> Simulation::getTextureCoords() const
{
    return _dp->getTexCoords();
}


QVector3D Simulation::jsonArray2Vec3(json11::Json const &vec_obj)
{
    std::vector<json11::Json> e = vec_obj.array_items();
    return QVector3D(e[0].number_value(), e[1].number_value(), e[2].number_value());
}
