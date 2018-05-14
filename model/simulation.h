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

class Simulation
{
    public:
        /**
         * @brief Simulation
         */
        Simulation();

        /**
         * d'tor
         */
        ~Simulation();

        /**
         * @brief runCalculations
         */
        void runCalculations();

        /**
         * @brief initialize
         * @param model_config
         */
        void initialize(ModelConfig *model_config);

        /**
         * @brief updateFromConfig
         * @param config
         */
        void updateFromConfig(ModelConfig* model_config);

        /**
         * @brief buildModel
         * @param model_config
         */
        void buildModel(ModelConfig *model_config);

        /**
         * @brief getScene
         */
        Scene getCurrentScene() const;

        /**
         * @brief Returns the current transformation mesh points
         * @return
         */
        std::vector<QVector3D> getTransformationMesh() const;

        /**
         * @brief Returns the current calculated texture coordinates
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
