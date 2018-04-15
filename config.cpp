#include "config.h"

Config::Config()
{}


bool Config::loadModelConfig(const QString &path) {
    QJsonObject object;
    if(loadConfig(object, path)) {
        parseModelConfig(object);
        return true;
    } else {
        return false;
    }
}

bool Config::loadUiConfig(const QString &path) {
    QJsonObject object;
    if(loadConfig(object, path)) {
        _ui_config = object;
        return true;
    } else {
        return false;
    }
}

ModelConfig* Config::getModelConfig() const {
    return _model_config;
}

QJsonObject Config::getcUiConfig() const {
    return _ui_config;
}

void Config::parseModelConfig(const QJsonObject &json_config) {

    QJsonObject projector_object = json_config["projector"].toObject();

    DomeProjectorConfig projector_config;
    projector_config.position = jsonArray2Vec3(projector_object["position"].toArray());
    projector_config.position = jsonArray2Vec3(projector_object["rotation"].toArray());
    projector_config.fov = (float) projector_object["fov"].toDouble();

    QJsonObject screen_object = projector_object["screen"].toObject();
    projector_config.screen_width = (int) screen_object["w"].toInt();
    projector_config.screen_height = (int) screen_object["h"].toInt();

    QJsonObject projector_dome_object = projector_object["dome"].toObject();
    projector_config.num_dome_rings = (int) projector_dome_object["num_rings"].toInt();
    projector_config.num_dome_ring_elements = (int) projector_dome_object["num_ring_elements"].toInt();

    QJsonObject projector_grid_object = projector_object["grid"].toObject();
    projector_config.num_grid_rings = (int) projector_grid_object["num_rings"].toInt();
    projector_config.num_grid_ring_elements = (int) projector_grid_object["num_ring_elements"].toInt();

    QJsonObject dome_object = json_config["dome"].toObject();
    SphereConfig dome_config;
    dome_config.position = jsonArray2Vec3(dome_object["position"].toArray());
    dome_config.rotation = jsonArray2Vec3(dome_object["rotation"].toArray());
    dome_config.radius = dome_object["radius"].toDouble();

    QJsonObject mirror_object = json_config["mirror"].toObject();
    SphereConfig mirror_config;
    mirror_config.position = jsonArray2Vec3(mirror_object["position"].toArray());
    mirror_config.rotation = jsonArray2Vec3(mirror_object["rotation"].toArray());
    mirror_config.radius = dome_object["radius"].toDouble();

    _model_config = new ModelConfig;
    _model_config->dome_projector = projector_config;
    _model_config->dome = dome_config;
    _model_config->mirror = mirror_config;
}


QVector3D Config::jsonArray2Vec3(const QJsonArray &vector_array) {
    float x = vector_array[0].toDouble();
    float y = vector_array[1].toDouble();
    float z = vector_array[2].toDouble();
    return QVector3D(x,y,z);
}


bool Config::loadConfig(QJsonObject &object,  const QString &filepath) {

    QString value;
    QFile file;
    file.setFileName(filepath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        value = file.readAll();
        file.close();
        qWarning() << value;
    } else {
        file.close();
        return false;
    }

    if(value.isEmpty() || value.isNull()) {
        return false;
    }

    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    object = document.object();

    return true;
}
