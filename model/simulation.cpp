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


void Simulation::runCalculations() {
    _dp->calculateDomeHitpoints(_mirror, _dome);
    _dp->calculateTransformationMesh();
}


void Simulation::updateFromConfig(ModelConfig *model_config) {

    float aspect_ratio = (float)model_config->dome_projector.screen_width /  model_config->dome_projector.screen_height;

    if(_dp) {
        _dp->updateFromConfig(model_config);
    } else {
        ProjectorFrustum *new_frustum = new ProjectorFrustum(aspect_ratio,
                                                             model_config->dome_projector.fov,
                                                             1.0f, 2.0f);
        _dp = new DomeProjector(new_frustum,
                                model_config->dome_projector.num_grid_rings,
                                model_config->dome_projector.num_grid_ring_elements,
                                model_config->dome_projector.position,
                                model_config->dome_projector.num_dome_rings,
                                model_config->dome_projector.num_dome_ring_elements);
    }


    if(_mirror) {
        _mirror->set_radius(model_config->mirror.radius);
        _mirror->set_position(model_config->mirror.position);
    } else {
        _mirror = new Sphere(model_config->mirror.radius,
                             model_config->mirror.position);
    }

    if(_dome) {
        _dome->set_radius(model_config->dome.radius);
        _dome->set_position(model_config->dome.position);
    } else {
        _dome = new Sphere(model_config->dome.radius,
                           model_config->dome.position);
    }

}


void Simulation::buildModel(ModelConfig *model_config) {
    updateFromConfig(model_config);
}


void Simulation::updateScene(Scene* scene) const {
    scene->dome_vertices = _dp->get_dome_vertices();
    scene->first_hits = _dp->get_first_hits();
    scene->sample_grid = _dp->get_sample_grid();
    scene->screen_points = _dp->get_screen_points();
    scene->far_corners = _dp->getFrustum()->getFarCorners();
    scene->near_corners = _dp->getFrustum()->getNearCorners();
}


QVector3D Simulation::jsonArray2Vec3(json11::Json const &vec_obj) {
    std::vector<json11::Json> e = vec_obj.array_items();
    return QVector3D(e[0].number_value(), e[1].number_value(), e[2].number_value());
}
