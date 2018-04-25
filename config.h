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
#include <QJsonParseError>
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
        int num_mesh_rings;
        int num_mesh_ring_elements;

        static DomeProjectorConfig fromJson(const QJsonObject &object);
};

struct SphereConfig
{
        float radius;
        QVector3D position;

        static SphereConfig fromJson(const QJsonObject &object);
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

        /**
         * @brief getModelConfigJson
         * @return
         */
        QJsonObject getModelConfigJson() const;

        /**
         * @brief getcUiConfig
         * @return
         */
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
        QVector3D jsonObject2Vec3(const QJsonObject &vector_array);

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
        QJsonObject _model_config_json;

        QJsonObject _ui_config;
};

#endif // CONFIG_H
