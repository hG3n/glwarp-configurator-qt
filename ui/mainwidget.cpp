#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include <map>

#include "mainwidget.h"
#include "mainwindow.h"
#include "config.h"
#include "configurationwidget.hpp"
#include "model/simulation.h"

MainWidget::MainWidget(MainWindow *mw)
    : _mainWindow(mw)

{

//    initSimulation();
    initLayout();
//    setUiValues(_config.getModelConfig());

    // get scene and create connection
//    _simulation.updateScene(_glWidget->getScene());

    // get model config
//    _model_config = _config.getModelConfig();

}


void MainWidget::initSimulation() {

    _config = Config();
    if(_config.loadModelConfig("../../../../glwarp-configuration-tool/_RES/configs/model.json")) {
        _simulation = Simulation();
        _simulation.buildModel(_config.getModelConfig());
        _simulation.runCalculations();
    } else {
        qDebug() << "There was an error loading the json";
    }

}

void MainWidget::initLayout() {

    _config.loadUiConfig("../../../../glwarp-configuration-tool/_RES/configs/ui.json");

    QJsonObject ui_config = _config.getcUiConfig();
    qDebug() << ui_config;

    _glWidget = new GLWidget(this);
    _configuration = new ConfigurationWidget(this);

    QHBoxLayout *root = new QHBoxLayout;
    root->setAlignment(Qt::AlignRight);
    root->setSpacing(0);
    root->addWidget(_glWidget);

    QScrollArea *scrollarea = new QScrollArea;
    scrollarea->setContentsMargins(0,0,0,0);

    scrollarea->setWidget(_configuration);

    root->addWidget(scrollarea);

//     set layout
    setLayout(root);
}

//void MainWidget::initProjectorUi(QVBoxLayout *root) {
//    // create projector group
//    QGroupBox *projector_group = new QGroupBox("Projector");
//    QVBoxLayout *projector_layout = new QVBoxLayout;


//    // position & rotation
//    QGroupBox *position_group = new QGroupBox("Position");
//    _projector_pos_edit = new PositionEdit(0.01, this);
//    position_group->setLayout(_projector_pos_edit->layout());

//    // rotation
//    QGroupBox *rotation_group = new QGroupBox("Rotation");
//    _projector_rot_edit = new PositionEdit(0.01, this);
//    rotation_group->setLayout(_projector_rot_edit->layout());

//    // Frustum
//    QGroupBox *frustum_group = new QGroupBox("Simulation Settings");
//    QFormLayout *frustum_form = new QFormLayout;
//    frustum_form->setFormAlignment(Qt::AlignRight);
//    _projector_fov = createSpinBox(1);
//    frustum_form->addRow("Fov", _projector_fov);
//    frustum_group->setLayout(frustum_form);

//    // Sample Grid
//    QGroupBox *grid_group = new QGroupBox("Sample Grid");
//    QFormLayout *grid_form = new QFormLayout;
//    _projector_sample_grid_ring = createSpinBox(1);
//    _projector_sample_grid_ring_elements = createSpinBox(1);
//    grid_form->addRow("Rings", _projector_sample_grid_ring);
//    grid_form->addRow("Ring Elements", _projector_sample_grid_ring_elements);
//    grid_group->setLayout(grid_form);

//    // Dome Grid
//    QGroupBox *dome_group = new QGroupBox("Dome Grid");
//    QFormLayout *dome_form = new QFormLayout;
//    _projector_dome_ring = createSpinBox(1);
//    _projector_dome_ring_elements = createSpinBox(1);
//    dome_form->addRow("Rings", _projector_dome_ring);
//    dome_form->addRow("Ring Elements", _projector_dome_ring_elements);
//    dome_group->setLayout(dome_form);

//    // add widgets to projector layout & group
//    projector_layout->addWidget(position_group);
//    projector_layout->addWidget(rotation_group);
//    projector_layout->addWidget(frustum_group);
//    projector_layout->addWidget(grid_group);
//    projector_layout->addWidget(dome_group);

//    projector_group->setLayout(projector_layout);
//    root->addWidget(projector_group);
//}

//void MainWidget::initMirrorUi(QVBoxLayout *root) {

//    // create projector group
//    QGroupBox *main_group = new QGroupBox("Mirror");
//    QVBoxLayout *main_layout = new QVBoxLayout;

//    // position
//    QGroupBox *position_group = new QGroupBox("Position");
//    _mirror_pos_edit= new PositionEdit(0.01, this);
//    position_group->setLayout(_mirror_pos_edit->layout());

//    // radius
//    QGroupBox *mirror_group = new QGroupBox("Mirror");
//    QFormLayout *mirror_form = new QFormLayout;
//    mirror_form->setFormAlignment(Qt::AlignRight);
//    _mirror_radius_edit = createSpinBox(0.1);
//    mirror_form->addRow("Radius", _mirror_radius_edit);
//    mirror_group->setLayout(mirror_form);

//    // main add
//    main_layout->addWidget(position_group);
//    main_layout->addWidget(mirror_group);

//    main_group->setLayout(main_layout);
//    root->addWidget(main_group);
//}

//void MainWidget::initDomeUi(QVBoxLayout *root) {
//    // create projector group
//    QGroupBox *main_group = new QGroupBox("Dome");
//    QVBoxLayout *main_layout = new QVBoxLayout;

//    // position
//    QGroupBox *position_group = new QGroupBox("Position");
//    _dome_pos_edit= new PositionEdit(0.01, this);
//    position_group->setLayout(_dome_pos_edit->layout());

//    // radius
//    QGroupBox *mirror_group = new QGroupBox("Dome");
//    QFormLayout *mirror_form = new QFormLayout;
//    mirror_form->setFormAlignment(Qt::AlignRight);
//    _dome_radius_edit = createSpinBox(0.1);
//    mirror_form->addRow("Radius", _dome_radius_edit);
//    mirror_group->setLayout(mirror_form);

//    // main add
//    main_layout->addWidget(position_group);
//    main_layout->addWidget(mirror_group);

//    main_group->setLayout(main_layout);
//    root->addWidget(main_group);
//}

//void MainWidget::setUiValues(ModelConfig *model_config) {

//    // set projector values
//    _projector_pos_edit->setValue(model_config->dome_projector.position);
//    _projector_rot_edit->setValue(model_config->dome_projector.rotation);
//    _projector_fov->setValue(model_config->dome_projector.fov);
//    _projector_sample_grid_ring->setValue(model_config->dome_projector.num_grid_rings);
//    _projector_sample_grid_ring_elements->setValue(model_config->dome_projector.num_grid_ring_elements);
//    _projector_dome_ring->setValue(model_config->dome_projector.num_dome_rings);
//    _projector_dome_ring_elements->setValue(model_config->dome_projector.num_dome_ring_elements);

//    // set mirror values
//    _mirror_pos_edit->setValue(model_config->mirror.position);
//    _mirror_radius_edit->setValue(model_config->mirror.radius);

//    // set dome values
//    _dome_pos_edit->setValue(model_config->dome.position);
//    _dome_radius_edit->setValue(model_config->dome.radius);
//}

//QPushButton* MainWidget::createButton(QString const& name, QString const& title) {
//    QPushButton *button = new QPushButton(name, this);
//    button->setText(title);
//    return button;
//}

//QDoubleSpinBox* MainWidget::createSpinBox(double step_size) {
//    QDoubleSpinBox* spinbox = new QDoubleSpinBox(this);
//    spinbox->setSingleStep(step_size);
//    return spinbox;
//}
