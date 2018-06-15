//
// Created by Hagen Hiller on 19/03/18.
//


#ifndef RAYCAST_FILE_WRITER_HPP
#define RAYCAST_FILE_WRITER_HPP

#include <QVector3D>

#include <string>
#include <vector>

#include "path.hpp"

class FileWriter {

public:

    /**
     * c'tor
     */
    FileWriter();

    /**
     * c'tor
     * @param path
     */
    FileWriter(Path path);

    /**
     * Sets the path
     * @param path
     */
    void setPath(Path const &path);

    /**
     * Saves std::vector of QVector3D to given filename
     * @param point_list
     * @param file
     */
    void writeVec3List(std::vector<QVector3D> const &point_list, std::string const &file);

private:
    Path _path;
};

#endif //RAYCAST_FILE_WRITER_HPP
