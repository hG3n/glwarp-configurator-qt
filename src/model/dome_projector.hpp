#ifndef RAYCAST_DOMEPROJECTOR_HPP
#define RAYCAST_DOMEPROJECTOR_HPP

#include <qmath.h>
#include <QVector3D>
#include <QMatrix>

#include <vector>
#include <map>
#include <ostream>
#include <limits>

#include "projector_frustum.hpp"
#include "sphere.hpp"
#include "config.h"

class DomeProjector {

    public:

        /**
         * Creates a dome projector object alongside the specified sample grid
         * @param _frustum
         * @param _screen
         * @param _grid_rings
         * @param _grid_ring_elements
         */
        DomeProjector(ProjectorFrustum *_frustum,
                      int _grid_rings,
                      int _grid_ring_elements,
                      QVector3D const &position,
                      int dome_rings,
                      int dome_ring_elements);

        /**
         * Destructor
         */
        ~DomeProjector();

        /**
         * @brief initialize
         */
        void initialize();

        /**
         * calculate the transformation mesh
         * @return std::vector<QVector3D> transformation mesh
         */
        void calculateTransformationMesh();

        /**
         * calculates hitpoints in the dome
         * @param mirror
         * @param dome
         */
        void calculateDomeHitpoints(Sphere *mirror, Sphere *dome);

        /**
         * @brief updateFromConfig
         * @param model_config
         */
        void updateFromConfig(ModelConfig *model_config);

        /**
         * Returns the projectors frustum
         * @return
         */
        ProjectorFrustum* getFrustum() const;

        /**
         * @brief translate
         * @param position
         */
        void translate(QVector3D position);

        /**
         * @brief Returns mesh coordinates
         * @return
         */
        std::vector<QVector3D> getMeshCoords() const;

        /**
         * @brief getTexCoords
         * @return
         */
        std::vector<QVector3D> getTexCoords() const;

    private:

        /**
         * generates a radial grid
         * @return
         */
        void generateRadialGrid();

        /**
         * @brief Generates the vertices of a half sphere by using the grid specified settings
         */
        void generateDomeVertices();

        /**
         * @brief findMinValues
         * @param vector
         * @return
         */
        QVector3D findMinValues(std::vector<QVector3D> vector);

        /**
         * @brief findMaxValues
         * @param vector
         * @return
         */
        QVector3D findMaxValues(std::vector<QVector3D> vector);

        /**
         * @brief mapToRange
         * @param value
         * @param in_min
         * @param in_max
         * @param out_min
         * @param out_max
         * @return
         */
        float mapToRange(float value, float in_min, float in_max, float out_min, float out_max);

    private:
        // members
        ProjectorFrustum *_frustum;

        QVector3D _position;

        int _grid_rings;
        int _grid_ring_elements;

        int _dome_rings;
        int _dome_ring_elements;

    public:
        std::vector<QVector3D> sample_grid;
        std::vector<QVector3D> first_hits;
        std::vector<QVector3D> second_hits;

        std::vector<QVector3D> dome_vertices;

        std::vector<QVector3D> mesh_coords;
        std::vector<QVector3D> texture_coords;

};


#endif //RAYCAST_DOMEPROJECTOR_HPP
