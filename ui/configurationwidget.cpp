#include "configurationwidget.hpp"

ConfigurationWidget::ConfigurationWidget(QWidget *parent) : QWidget(parent)
{
    initLayout();

//    connect(_recalculate_btn, &QPushButton::released,
//            this, &MainWidget::onRecalculateButtonPressed);
}

//void MainWidget::onRecalculateButtonPressed() {
//    _simulation.buildModel(_model_config);
//    _simulation.runCalculations();
//}

void ConfigurationWidget::initLayout() {

//    QJsonObject config;
//    config["title"] = "title";

//    QJsonArray value_list;

//    QJsonObject element1;
//    element1["title"] = "first";
//    element1["attribute"] = "";
//    element1["precision"] = 0.1;

//    QJsonObject element2;
//    element2["title"] = "second";
//    element2["attribute"] = "another";
//    element2["precision"] = 0.1;

//    value_list.append(element1);
//    value_list.append(element2);

//    config["elements"] = value_list;

//    auto p = new PropertyEdit(config, this);
//    qDebug() << p->getValues();

    // create new layout
    QVBoxLayout *root = new QVBoxLayout;
    root->setSpacing(0);
    root->setAlignment(Qt::AlignRight);
    root->setContentsMargins(0,0,0,0);

    _projector_config_widget = new ProjectorConfigWidget(this);
    root->addWidget(_projector_config_widget);

    // add button
    _recalculate_btn = createButton("recalculate_button", "Recalculate");
    root->addWidget(_recalculate_btn);

    // set layout
    setLayout(root);
}

//void ConfigurationWidget::initProjectorUi(QVBoxLayout *root) {
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

//void ConfigurationWidget::initMirrorUi(QVBoxLayout *root) {

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

//void ConfigurationWidget::initDomeUi(QVBoxLayout *root) {
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


QPushButton* ConfigurationWidget::createButton(QString const& name, QString const& title) {
    QPushButton *button = new QPushButton(name, this);
    button->setText(title);
    return button;
}

QDoubleSpinBox* ConfigurationWidget::createSpinBox(double step_size) {
    QDoubleSpinBox* spinbox = new QDoubleSpinBox(this);
    spinbox->setSingleStep(step_size);
    return spinbox;
}
