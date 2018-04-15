#include "vector3edit.hpp"

Vector3Edit::Vector3Edit(QWidget *parent)
    : QWidget(parent)
{
    initLayout(0.1);
}

Vector3Edit::Vector3Edit(double step_size, QWidget *parent)
    : QWidget(parent)
{
    initLayout(step_size);
}

void Vector3Edit::initLayout(double step_size = 0.1) {
    QFormLayout *form = new QFormLayout;
    form->setFormAlignment(Qt::AlignRight);

    _x_edit = new QDoubleSpinBox(this);
    _x_edit->setSingleStep(step_size);

    _y_edit = new QDoubleSpinBox(this);
    _y_edit->setSingleStep(step_size);

    _z_edit = new QDoubleSpinBox(this);
    _z_edit->setSingleStep(step_size);

    form->addRow("X", _x_edit);
    form->addRow("Y", _y_edit);
    form->addRow("Z", _z_edit);

    setLayout(form);
}

void Vector3Edit::setStepSize(double step_size) {
    _x_edit->setSingleStep(step_size);
    _y_edit->setSingleStep(step_size);
    _z_edit->setSingleStep(step_size);
}

void Vector3Edit::setValue(const QVector3D &value) {
    _x_edit->setValue(value.x());
    _y_edit->setValue(value.y());
    _z_edit->setValue(value.z());
}

QVector3D Vector3Edit::getValue() const {
    QVector3D pos(_x_edit->value(), _y_edit->value(), _z_edit->value());
    return pos;
}


