//
// Created by Hagen Hiller on 09/01/18.
//

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
        std::vector<QVector3D> calculateTransformationMesh();

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
         * Returns a std::vec containing the radial sample grid.
         * @return
         */
        std::vector<QVector3D> const &get_sample_grid() const;

        /**
         * Returns a std::vector containing all first hitpoints supposed to be on the mirrors surface.
         * @return
         */
        std::vector<QVector3D> const &get_first_hits() const;

        /**
         * Returns a std::vector containing all second hitpoints supposed to be within the dome.
         * @return
         */
        std::vector<QVector3D> const &get_second_hits() const;

        /**
         * Returns a std::vector containing the vertices for a half sphere.
         * @return
         */
        std::vector<QVector3D> const &get_dome_vertices() const;

        /**
         * Returns a std::vector containing vertices of the final warping mesh
         * @return
         */
        std::vector<QVector3D> const &get_screen_points() const;

        /**
         * Returns a std::vector containing vertices of the final warping mesh
         * @return
         */
        std::vector<QVector3D> const &get_texture_coords() const;

        /**
         * ostream
         * @param os
         * @param projector
         * @return
         */
        friend std::ostream &operator<<(std::ostream &os, const DomeProjector &projector);

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

        std::vector<QVector3D> _sample_grid;
        std::vector<QVector3D> _first_hits;
        std::vector<QVector3D> _second_hits;

        std::vector<QVector3D> _dome_vertices;

        std::vector<QVector3D> _screen_points;
        std::vector<QVector3D> _texture_coords;
};


#endif //RAYCAST_DOMEPROJECTOR_HPP
