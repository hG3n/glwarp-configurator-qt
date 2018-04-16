#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include <map>

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
    //    setUiValues(_config.getModelConfig());

    connect(_configurator, &Configurator::valueUpdate,
            this, &MainWidget::onValueUpdate);

    // get scene and create connection
    _simulation.updateScene(_glWidget->getScene());


    QMap<QString, int> map;
    map["first"] = 1;
    map["second"] = 2;
    map["third"] = 3;

    for(auto element: map.keys()) {
        qDebug() << element;
    }


}

void MainWidget::onValueUpdate(QJsonObject new_values) {
    QJsonObject projector_obj = new_values["projector"].toObject();
    DomeProjectorConfig projector_config = DomeProjectorConfig::fromJson(projector_obj);

    QJsonObject mirror_obj = new_values["mirror"].toObject();
    SphereConfig mirror_config = SphereConfig::fromJson(mirror_obj);

    QJsonObject dome_obj = new_values["dome"].toObject();
    SphereConfig dome_config = SphereConfig::fromJson(dome_obj);

    ModelConfig model_config;
    model_config.dome_projector = projector_config;
    model_config.dome = dome_config;
    model_config.mirror = mirror_config;
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
    scrollarea->setFixedWidth(210);
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

void MainWidget::initSimulation() {
    _simulation = Simulation();
    _simulation.buildModel(_config.getModelConfig());
    _simulation.runCalculations();
}

QVector3D MainWidget::vec3fromJson(const QJsonObject &object) const{
    return QVector3D(object["x"].toDouble(), object["y"].toDouble(), object["z"].toDouble());
}
