#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>

#include <QString>
#include <QVector3D>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

#include "lib/json11.hpp"

struct DomeProjectorConfig
{
        QVector3D position;
        QVector3D rotation;
        float fov;
        int screen_width;
        int screen_height;
        int num_grid_rings;
        int num_grid_ring_elements;
        int num_dome_rings;
        int num_dome_ring_elements;
};

struct SphereConfig
{
        float radius;
        QVector3D position;
        QVector3D rotation;
};

struct ModelConfig
{
        DomeProjectorConfig dome_projector;
        SphereConfig dome;
        SphereConfig mirror;
};


class Config
{
    public:
        Config();

        /**
         * @brief loadModelConfig
         * @return
         */
        bool loadModelConfig(const QString &path);

        /**
         * @brief loadUiConfig
         * @param path
         * @return
         */
        bool loadUiConfig(const QString &path);

        /**
         * @brief getModelConfig
         * @return
         */
        ModelConfig* getModelConfig() const;

        QJsonObject getcUiConfig() const;

    private:
        /**
         * @brief parseConfigFile
         * @param json_config
         */
        void parseModelConfig(const QJsonObject &json_config);

        /**
         * @brief jsonArray2Vec3
         * @param vec_obj
         * @return
         */
        QVector3D jsonArray2Vec3(const QJsonArray & vector_array);

        /**
         * @brief loadConfig
         * @param object
         * @param filename
         * @return
         */
        bool loadConfig(QJsonObject &object, const QString &filename);

    private:
        QString _config_path;

        ModelConfig *_model_config;

        QJsonObject _ui_config;
};

#endif // CONFIG_H
