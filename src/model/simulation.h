#ifndef SIMULATION_H
#define SIMULATION_H

#include <QVector3D>

#include <vector>
#include <map>
#include <string>

#include "dome_projector.hpp"
#include "projector_frustum.hpp"
#include "sphere.hpp"
#include "lib/json11.hpp"
#include "config.h"

/**
 * @brief The Scene struct contains all values needed by the GlWidget class to render a simulated configuration.
 */
struct Scene {
    std::vector<QVector3D> sample_grid;
    std::vector<QVector3D> first_hits;
    std::vector<QVector3D> second_hits;

    std::vector<QVector3D> dome_vertices;

    std::vector<QVector3D> screen_points;
    std::vector<QVector3D> texture_coords;

    std::map<ProjectorFrustum::Corner, QVector3D> near_corners;
    std::map<ProjectorFrustum::Corner, QVector3D> far_corners;
};

/**
 * @brief The Simulation class handles single model simulation passes.
 * The Simulation is a mere middleman handling connections to the model
 * from the outside as well as passing computed results to outter classes
 * and interfaces.
 *
 */
class Simulation
{
    public:
        Simulation();
        ~Simulation();

        /**
         * @brief Runs a Simulation pass.
         */
        void runCalculations();

        /**
         * @brief Initializes main Simulation components.
         *
         * Components created are:
         *   the first reflection mirror <Sphere>
         *   the final reflection medium <Sphere>
         *   the dome projector maintaining the calculations <DomeProjector>
         *
         * @param model_config
         */
        void initialize(ModelConfig *model_config);

        /**
         * @brief Updates the Simulation from given json configuration.
         * @param config
         */
        void updateFromConfig(ModelConfig* model_config);

        /**
         * @brief Builds the model.
         * @param model_config
         */
        void buildModel(ModelConfig *model_config);

        /**
         * @brief Returns a scene dto.
         */
        Scene getCurrentScene() const;

        /**
         * @brief Returns the current transformation mesh points.
         * @return
         */
        std::vector<QVector3D> getTransformationMesh() const;

        /**
         * @brief Returns the current calculated texture coordinates.
         * @return
         */
        std::vector<QVector3D> getTextureCoords() const;

    private:
        /**
         * @brief jsonArray2Vec3
         * @param vec_obj
         * @return
         */
        QVector3D jsonArray2Vec3(json11::Json const &vec_obj);

        // model vars
        DomeProjector *_dp;
        Sphere * _mirror;
        Sphere * _dome;
};


#endif // SIMULATION_H
