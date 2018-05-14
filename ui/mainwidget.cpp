#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include "mainwidget.h"
#include "mainwindow.h"
#include "config.h"
#include "propertyeditgroup.h"
#include "model/simulation.h"

MainWidget::MainWidget(MainWindow *mw)
    : _mainWindow(mw)

{
    loadConfigs();
    initLayout();

    initSimulation();

    connect(_configurator, &Configurator::valueUpdate,
            this, &MainWidget::onValueUpdate);

    setUiValues(_config.getModelConfigJson());

    _glWarpWidget = new GLWarpWidget();

    // todo: reimplement this
    // get scene pointer
    //    _simulation.updateScene(_glWidget->getScene());
}

void MainWidget::onValueUpdate(QJsonObject new_values) {
    // get configs
    QJsonObject projector_obj = new_values["projector"].toObject();
    DomeProjectorConfig projector_config = DomeProjectorConfig::fromJson(projector_obj);

    QJsonObject mirror_obj = new_values["mirror"].toObject();
    SphereConfig mirror_config = SphereConfig::fromJson(mirror_obj);

    QJsonObject dome_obj = new_values["dome"].toObject();
    SphereConfig dome_config = SphereConfig::fromJson(dome_obj);

    // create model config
    ModelConfig *config = new ModelConfig;
    config->dome_projector = projector_config;
    config->dome = dome_config;
    config->mirror = mirror_config;

    // get new simulation values and update gl
    _simulation.updateFromConfig(config);
    delete config;
    _simulation.runCalculations();
    Scene s = _simulation.getCurrentScene();
    _glWidget->setScene(s);

    // update the warp view as well
    std::vector<QVector3D> mesh_coords = _simulation.getTransformationMesh();
    std::vector<QVector3D> tex_coords = _simulation.getTextureCoords();
    _glWarpWidget->updateValues(mesh_coords, tex_coords);
}

void MainWidget::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Space) {

        std::vector<QVector3D> mesh_coords = _simulation.getTransformationMesh();
        std::vector<QVector3D> tex_coords = _simulation.getTextureCoords();

        _glWarpWidget->updateValues(mesh_coords, tex_coords);

        _glWarpWidget->show();
    }
}

void MainWidget::initLayout() {
    setContentsMargins(0,0,0,0);

    QHBoxLayout *root = new QHBoxLayout;
    root->setContentsMargins(0,0,0,0);
    root->setAlignment(Qt::AlignRight);
    root->setSpacing(0);

    _glWidget = new GLWidget(this);
    root->addWidget(_glWidget);

    QScrollArea *scrollarea = new QScrollArea;
    scrollarea->setFixedWidth(250);
    scrollarea->setAlignment(Qt::AlignCenter);
    scrollarea->setContentsMargins(0,0,0,0);

    _configurator = new Configurator(_ui_config, this);
    scrollarea->setWidget(_configurator);

    root->addWidget(scrollarea);

    setLayout(root);
}

void MainWidget:: loadConfigs() {
    _config = Config();
    if(_config.loadModelConfig("../../../../glwarp-configuration-tool/_RES/configs/model.json")) {
        _model_config = _config.getModelConfig();
    } else {
        qDebug() << "Error laoding model config!";
    }

    if(_config.loadUiConfig("../../../../glwarp-configuration-tool/_RES/configs/ui.json")) {
        _ui_config = _config.getcUiConfig();
    } else {
        qDebug() << "Error loading UI config!";
    }
}

void MainWidget::setUiValues(QJsonObject model_config) {
    _configurator->setValues(model_config);
}

void MainWidget::initSimulation() {
    _simulation = Simulation();
    _simulation.initialize(_config.getModelConfig());

    _simulation.runCalculations();
    Scene s = _simulation.getCurrentScene();

    _glWidget->setScene(s);
}

QVector3D MainWidget::vec3fromJson(const QJsonObject &object) const{
    return QVector3D(object["x"].toDouble(), object["y"].toDouble(), object["z"].toDouble());
}
