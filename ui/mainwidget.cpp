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

    // get scene and create connection
    _simulation.updateScene(_glWidget->getScene());
}

bool MainWidget:: loadConfigs() {
    _config = Config();
    if(_config.loadModelConfig("../../../../glwarp-configuration-tool/_RES/configs/model.json")) {
        _model_config = _config.getModelConfig();
    } else {
        qDebug() << "Error laoding model config!";
        return false;
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

void MainWidget::initLayout() {

    setContentsMargins(0,0,0,0);

    QHBoxLayout *root = new QHBoxLayout;
    root->setContentsMargins(0,0,0,0);
    root->setAlignment(Qt::AlignRight);
    root->setSpacing(0);

    _configurator = new Configurator(_ui_config, this);
    QScrollArea *scrollarea = new QScrollArea;
    scrollarea->setFixedWidth(210);
    scrollarea->setAlignment(Qt::AlignCenter);
    scrollarea->setContentsMargins(0,0,0,0);
    scrollarea->setWidget(_configurator);

    _glWidget = new GLWidget(this);

    root->addWidget(_glWidget);
    root->addWidget(scrollarea);

    //     set layout
    setLayout(root);
}
