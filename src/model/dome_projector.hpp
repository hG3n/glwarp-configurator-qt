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

/**
 * @brief The DomeProjector class describes a projector specialized on calculating a warping geometry.
 *
 * The dome projector calculates a, for the given model accurate projection mesh.
 */
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
         * @brief Calculates the transformation mesh using data previously simulated.
         *
         *
         *
         * @return std::vector<QVector3D> transformation mesh
         */
        void calculateTransformationMesh();

        /**
         * @brief Performs a raycast, aiming to hit the dome using the mirror as reflection medium.
         * @param mirror
         * @param dome
         */
        void calculateDomeHitpoints(Sphere *mirror, Sphere *dome);

        /**
         * @brief Updates internal values from given config.
         *
         * It thereby regenerates the radial sampling grid as well as the dome
         * grid with the specified values.
         * @param model_config
         */
        void updateFromConfig(ModelConfig *model_config);

        /**
         * Returns the projectors frustum.
         * @return
         */
        ProjectorFrustum* getFrustum() const;

        /**
         * @brief Alter the DomeProjector's position.
         * @param position
         */
        void translate(QVector3D position);

        /**
         * @brief Returns calculated mesh coordinates.
         * @return
         */
        std::vector<QVector3D> getMeshCoords() const;

        /**
         * @brief Returns calculated texture coordinates.
         * @return
         */
        std::vector<QVector3D> getTexCoords() const;

    private:

        /**
         * @brief Generates the radial sampling grid on the Frustums near clipping plane.
         * @return
         */
        void generateRadialGrid();

        /**
         * @brief Generates the vertices of a half sphere by using the grid specified settings.
         */
        void generateDomeVertices();

        /**
         * @brief Finds minimum values for all three axis of a list of QVectors.
         * @param vector
         * @return
         */
        QVector3D findMinValues(std::vector<QVector3D> vector);

        /**
         * @brief Finds maximum values for all three axis of a list of QVectors.
         * @param vector
         * @return
         */
        QVector3D findMaxValues(std::vector<QVector3D> vector);

        /**
         * @brief Maps a value in a given range to the desired output range.
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
        QVector3D _rotation;

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
