#include <QGroupBox>

#include "projectorconfigwidget.hpp"

ProjectorConfigWidget::ProjectorConfigWidget(QWidget *parent) : QWidget(parent)
{
    initLayout();
}

void ProjectorConfigWidget::initLayout() {

    // create projector group
    QGroupBox *root = new QGroupBox("Projector");
    QVBoxLayout *projector_layout = new QVBoxLayout;

    // position & rotation
    QGroupBox *position_group = new QGroupBox("Position");
    _projector_pos_edit = new Vector3Edit(0.01, this);
    position_group->setLayout(_projector_pos_edit->layout());

    // rotation
    QGroupBox *rotation_group = new QGroupBox("Rotation");
    _projector_rot_edit = new Vector3Edit(0.01, this);
    rotation_group->setLayout(_projector_rot_edit->layout());

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

    // add widgets to projector layout & group
    projector_layout->addWidget(position_group);
    projector_layout->addWidget(rotation_group);
//    projector_layout->addWidget(frustum_group);
//    projector_layout->addWidget(grid_group);
//    projector_layout->addWidget(dome_group);

    root->setLayout(projector_layout);

    setLayout(root->layout());
}
