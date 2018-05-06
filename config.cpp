#include <string>

#include "config.h"

DomeProjectorConfig DomeProjectorConfig::fromJson(const QJsonObject &object) {

    QJsonObject position_obj = object["position"].toObject();
    QVector3D position = QVector3D( position_obj["x"].toDouble(),  position_obj["y"].toDouble(),  position_obj["z"].toDouble());

    QJsonObject rotation_obj = object["rotation"].toObject();
    QVector3D rotation = QVector3D( rotation_obj["x"].toDouble(),  rotation_obj["y"].toDouble(),  rotation_obj["z"].toDouble());

    double fov = object["fov"].toObject()["fov"].toDouble();

    int screen_w = object["screen"].toObject()["w"].toInt();
    int screen_h = object["screen"].toObject()["h"].toInt();

    int grid_rings = object["grid"].toObject()["rings"].toInt();
    int grid_ring_elements = object["grid"].toObject()["ring_elements"].toInt();

    int mesh_rings = object["mesh"].toObject()["rings"].toInt();
    int mesh_ring_elements = object["mesh"].toObject()["ring_elements"].toInt();

    DomeProjectorConfig config;
    config.position = position;
    config.rotation = rotation;
    config.fov = fov;
    config.screen_width = screen_w;
    config.screen_height = screen_h;
    config.num_grid_rings = grid_rings;
    config.num_grid_ring_elements = grid_ring_elements;
    config.num_mesh_rings = mesh_rings;
    config.num_mesh_ring_elements = mesh_ring_elements;

    return config;
}


SphereConfig SphereConfig::fromJson(const QJsonObject &object) {
    QJsonObject position_obj = object["position"].toObject();

    QVector3D position = QVector3D( position_obj["x"].toDouble(),  position_obj["y"].toDouble(),  position_obj["z"].toDouble());
    double radius = object["radius"].toObject()["radius"].toDouble();

    SphereConfig config;
    config.position = position;
    config.radius = radius;

    return config;
}


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

QJsonObject Config::getModelConfigJson() const {
    return _model_config_json;
}

QJsonObject Config::getcUiConfig() const {
    return _ui_config;
}

void Config::parseModelConfig(const QJsonObject &json_config) {

    _model_config_json = json_config;

    QJsonObject projector_object = json_config["projector"].toObject();

    DomeProjectorConfig projector_config;
    projector_config.position = jsonObject2Vec3(projector_object["position"].toObject());
    projector_config.rotation = jsonObject2Vec3(projector_object["rotation"].toObject());
    projector_config.fov = (float) projector_object["fov"].toObject()["fov"].toDouble();

    QJsonObject screen_object = projector_object["screen"].toObject();
    projector_config.screen_width = (int) screen_object["w"].toInt();
    projector_config.screen_height = (int) screen_object["h"].toInt();

    QJsonObject projector_mesh_object = projector_object["mesh"].toObject();
    projector_config.num_mesh_rings = (int) projector_mesh_object["rings"].toInt();
    projector_config.num_mesh_ring_elements = (int) projector_mesh_object["ring_elements"].toInt();

    QJsonObject projector_grid_object = projector_object["grid"].toObject();
    projector_config.num_grid_rings = (int) projector_grid_object["rings"].toInt();
    projector_config.num_grid_ring_elements = (int) projector_grid_object["ring_elements"].toInt();

    QJsonObject dome_object = json_config["dome"].toObject();
    SphereConfig dome_config;
    dome_config.position = jsonObject2Vec3(dome_object["position"].toObject());
    dome_config.radius = dome_object["radius"].toObject()["radius"].toDouble();

    QJsonObject mirror_object = json_config["mirror"].toObject();
    SphereConfig mirror_config;
    mirror_config.position = jsonObject2Vec3(mirror_object["position"].toObject());
    mirror_config.radius = mirror_object["radius"].toObject()["radius"].toDouble();

    _model_config = new ModelConfig;
    _model_config->dome_projector = projector_config;
    _model_config->dome = dome_config;
    _model_config->mirror = mirror_config;
}


QVector3D Config::jsonObject2Vec3(const QJsonObject &object) {
    float x = object["x"].toDouble();
    float y = object["y"].toDouble();
    float z = object["z"].toDouble();
    return QVector3D(x,y,z);
}


bool Config::loadConfig(QJsonObject &object,  const QString &filepath) {

    QString value;
    QFile file;
    file.setFileName(filepath);
    if(file.open(QIODevice::ReadOnly)) {
        value = file.readAll();
        file.close();
    } else {
        file.close();
        return false;
    }

    if(value.isEmpty() || value.isNull()) {
        return false;
    }

    QJsonParseError *error = new QJsonParseError;
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8(), error);

    qDebug() << "Parsing JSON:" << "'" << filepath << "'" << error->errorString();

    object = document.object();

    return true;
}
