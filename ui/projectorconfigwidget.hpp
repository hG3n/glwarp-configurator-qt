#ifndef PROJECTORCONFIGWIDGET_HPP
#define PROJECTORCONFIGWIDGET_HPP

#include <QWidget>
#include <QDoubleSpinBox>

#include "ui/vector3edit.hpp"
#include "vector3edit.hpp"
class ProjectorConfigWidget : public QWidget
{
        Q_OBJECT
    public:
        ProjectorConfigWidget(QWidget *parent = 0);

    signals:

    public slots:

    private:
        void initLayout();

    private:
        Vector3Edit *_projector_pos_edit;
        Vector3Edit *_projector_rot_edit;
        QDoubleSpinBox *_projector_fov;
        QDoubleSpinBox *_projector_sample_grid_ring;
        QDoubleSpinBox *_projector_sample_grid_ring_elements;
        QDoubleSpinBox *_projector_dome_ring;
        QDoubleSpinBox *_projector_dome_ring_elements;
};

#endif // PROJECTORCONFIGWIDGET_HPP
